package com.rust.util;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.PipelineManager;
import com.rust.model.PipelineState;
import glm.vec._2.Vec2;
import glm.vec._3.Vec3;
import org.lwjgl.assimp.*;
import org.lwjgl.*;
import org.lwjgl.stb.STBImage;

import java.nio.*;
import java.util.ArrayList;
import java.util.Objects;

import static org.lwjgl.opengl.GL11C.*;
import static org.lwjgl.opengl.GL11C.GL_LINEAR;
import static org.lwjgl.opengl.GL11C.GL_LINEAR_MIPMAP_LINEAR;
import static org.lwjgl.opengl.GL11C.GL_REPEAT;
import static org.lwjgl.opengl.GL11C.GL_RGB;
import static org.lwjgl.opengl.GL11C.GL_RGBA;
import static org.lwjgl.opengl.GL11C.GL_TEXTURE_2D;
import static org.lwjgl.opengl.GL11C.GL_TEXTURE_MAG_FILTER;
import static org.lwjgl.opengl.GL11C.GL_TEXTURE_MIN_FILTER;
import static org.lwjgl.opengl.GL11C.GL_TEXTURE_WRAP_S;
import static org.lwjgl.opengl.GL11C.GL_TEXTURE_WRAP_T;
import static org.lwjgl.opengl.GL11C.GL_UNSIGNED_BYTE;
import static org.lwjgl.opengl.GL33C.*;

public class Model {

    private final ArrayList<Mesh> meshes = new ArrayList<>();
    private final ArrayList<Texture> textures_loaded = new ArrayList<>();

    private String directory;

    public void load(String resourcePath, String textureDir) throws Exception {
        load(resourcePath,textureDir,Assimp.aiProcess_JoinIdenticalVertices | Assimp.aiProcess_Triangulate | Assimp.aiProcess_FlipUVs | Assimp.aiProcess_GenSmoothNormals | Assimp.aiProcess_CalcTangentSpace);
    }
    public void load(String resoursePath,String textureDir,int flags) throws Exception {
        AIScene aiScene = Assimp.aiImportFile(resoursePath,flags);
        if(aiScene == null){
            throw new Exception("Model in:"+resoursePath+"may not exist.");
        }
        directory = textureDir;
        processNode(Objects.requireNonNull(aiScene.mRootNode()),aiScene);
    }

    private void processNode(AINode node,AIScene scene) throws Exception {
        PointerBuffer pointerBuffer = scene.mMeshes();
        for (int i=0;i<node.mNumMeshes();i++){
            AIMesh mesh = AIMesh.create(pointerBuffer.get(node.mMeshes().get(i)));
            meshes.add(processMesh(mesh,scene));
        }
        PointerBuffer childrenBuffer = node.mChildren();
        for(int i=0;i<node.mNumChildren();i++){
            AINode child = AINode.create(childrenBuffer.get(i));
            processNode(child ,scene);
        }
    }

    private Mesh processMesh(AIMesh mesh,AIScene scene) throws Exception {
        ArrayList<Vertex> vertices = new ArrayList<>();
        ArrayList<Integer> indices = new ArrayList<>();
        ArrayList<Texture> textures = new ArrayList<>();
        AIVector3D.Buffer texCoords = null;
        if(mesh.mTextureCoords().get(0)!=0){
            texCoords = new AIVector3D.Buffer(mesh.mTextureCoords().get(0),mesh.mNumVertices());
        }
        for (int i=0;i<mesh.mNumVertices();i++){
            Vertex vertex = new Vertex();
            AIVector3D temp = mesh.mVertices().get(i);
            vertex.Position = new Vec3(temp.x(),temp.y(),temp.z());
            if(mesh.mNormals()!= null){
                temp = mesh.mNormals().get(i);
                vertex.Normal = new Vec3(temp.x(),temp.y(),temp.z());
            }
            if(texCoords != null){
                AIVector3D currentTexCoords = texCoords.get();
                vertex.TexCoords = new Vec2(currentTexCoords.x(),currentTexCoords.y());
            }else{
                vertex.TexCoords = new Vec2(0,0);
            }
            if(mesh.mTangents()!=null){
                temp = mesh.mTangents().get(i);
                vertex.Tangent = new Vec3(temp.x(),temp.y(),temp.z());
            }
            if(mesh.mBitangents()!=null){
                temp = mesh.mBitangents().get(i);
                vertex.BitTagnent = new Vec3(temp.x(),temp.y(),temp.z());
            }
            vertices.add(vertex);
        }
        for(int i=0;i<mesh.mNumFaces();i++){
            AIFace face = mesh.mFaces().get(i);
            for(int j=0;j<face.mNumIndices();j++){
                indices.add(face.mIndices().get(j));
            }
        }
        AIMaterial material = AIMaterial.create(scene.mMaterials().get(mesh.mMaterialIndex()));

        ArrayList<Texture> diffuseMaps = loadMaterialTextures(material,Assimp.aiTextureType_DIFFUSE,"texture_diffuse");
        textures.addAll(diffuseMaps);
        ArrayList<Texture> specularMaps = loadMaterialTextures(material,Assimp.aiTextureType_SPECULAR,"texture_specular");
        textures.addAll(specularMaps);
        ArrayList<Texture> normalMaps = loadMaterialTextures(material,Assimp.aiTextureType_NORMALS,"texture_normal");
        textures.addAll(normalMaps);
        ArrayList<Texture> heightMaps = loadMaterialTextures(material,Assimp.aiTextureType_HEIGHT,"texture_height");
        textures.addAll(heightMaps);

        return new Mesh(vertices,indices,textures);
    }

    public void Draw(Shader shader){
        for (Mesh mesh: meshes
             ) {
            mesh.Draw(shader);
        }
    }


    private ArrayList<Texture> loadMaterialTextures(AIMaterial mat, int type, String typeName) throws Exception {
        ArrayList<Texture> textures = new ArrayList<>();
        AIString path = AIString.calloc();
        for(int i=0;i<Assimp.aiGetMaterialTextureCount(mat,type);i++){
            Assimp.aiGetMaterialTexture(mat,type,i,path,(IntBuffer)null,null,null,null,null,null);
            String str = path.dataString();
            boolean skip = false;
            for (Texture value : textures_loaded) {
                if (value.path.equals(str)) {
                    textures.add(value);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture = new Texture();
                texture.id = TextureFromFile(str,directory).get(0);
                texture.type = typeName;
                texture.path = str;
                textures.add(texture);
                textures_loaded.add(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    public static IntBuffer TextureFromFile(String str,String directory) throws Exception {
        String filename = directory + "/" + str;
        IntBuffer texture_id = GLBuffers.newDirectIntBuffer(1);
        glGenTextures(texture_id);

        IntBuffer width = GLBuffers.newDirectIntBuffer(1);
        IntBuffer height = GLBuffers.newDirectIntBuffer(1);
        IntBuffer nrComponents = GLBuffers.newDirectIntBuffer(1);

        ByteBuffer data = STBImage.stbi_load(filename,width,height,nrComponents,0);
        if(data!=null){
            int format = GL_RED;
            if(nrComponents.get(0) == 3){
                format = GL_RGB;
            }else{
                format = GL_RGBA;
            }
            glBindTexture(GL_TEXTURE_2D,texture_id.get(0));
            glTexImage2D(GL_TEXTURE_2D,0,format,width.get(0),height.get(0),0,format,GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            STBImage.stbi_image_free(data);
        }else{
            throw new Exception("Fail to load image " + filename);
        }

        return texture_id;
    }
}
