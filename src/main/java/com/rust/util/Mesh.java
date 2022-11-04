package com.rust.util;

import com.jogamp.opengl.GL3;
import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.PipelineManager;
import org.lwjgl.opengl.GL30;

import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.opengl.GL33C.*;

public class Mesh {
    public List<Vertex> vertices = new ArrayList<>();
    public List<Integer> indices = new ArrayList<>();
    public List<Texture> textures = new ArrayList<>();
    public Mesh(List<Vertex> vertices, List<Integer> indices, List<Texture> textures){
        this.vertices = vertices;
        this.indices = indices;
        this.textures = textures;
        setupMesh();
    }

    private final IntBuffer VAO = GLBuffers.newDirectIntBuffer(1);
    private final IntBuffer VBO = GLBuffers.newDirectIntBuffer(1);
    private final IntBuffer EBO = GLBuffers.newDirectIntBuffer(1);


    private void setupMesh()
    {
        glGenVertexArrays(VAO);
        glGenBuffers(VBO);
        glGenBuffers(EBO);

        glBindVertexArray(VAO.get(0));
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO.get(0));

        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER,Vertex.toFb(vertices), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO.get(0));
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLBuffers.newDirectIntBuffer(toIndiceArray()), GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(Semantic.Attr.POSITION);
        glVertexAttribPointer(Semantic.Attr.POSITION, 3, GL_FLOAT,false, Vertex.BYTES(), Vertex.offsetBy(VertexAttrib.Position));
        // vertex normals
        glEnableVertexAttribArray(Semantic.Attr.NORMAL);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, Vertex.BYTES(), Vertex.offsetBy(VertexAttrib.Normal));
        // vertex texture coords
        glEnableVertexAttribArray(Semantic.Attr.TEX_COORD);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, Vertex.BYTES(), Vertex.offsetBy(VertexAttrib.TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(Semantic.Attr.TANGENT);
        glVertexAttribPointer(3, 3, GL_FLOAT, false, Vertex.BYTES(), Vertex.offsetBy(VertexAttrib.Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(Semantic.Attr.BITANGENT);
        glVertexAttribPointer(4, 3, GL_FLOAT, false, Vertex.BYTES(), Vertex.offsetBy(VertexAttrib.BitTangent));

        glBindVertexArray(0);
    }

    private int[] toIndiceArray(){
        int[] ret = new int[indices.size()];
        int index=0;
        for (int indice: indices
             ) {
            ret[index++] = indice;
        }
        return ret;
    }

    public void Draw(Shader shader)
    {
        int diffuseNr = 1;
        int specularNr = 1;
        for (int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            String number = "";
            String name = textures.get(i).type;
            if (name.equals("texture_diffuse")){
                number = Integer.toString(diffuseNr);
                diffuseNr++;
            }
            else if (name.equals("texture_specular")) {
                number = Integer.toString(specularNr);
                specularNr++;
            }
            shader.setInt("material_" + name + number, i);
            glBindTexture(GL_TEXTURE_2D, textures.get(i).id);
        }
        glActiveTexture(GL_TEXTURE0);
        int generatedMap = PipelineManager.shared.output.diffuse_map;
        if(generatedMap > 0) {
            shader.setInt("material_texture_diffuse0", 0);
            glBindTexture(GL_TEXTURE_2D, generatedMap);
        }
        // draw mesh
        glBindVertexArray(VAO.get(0));
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
