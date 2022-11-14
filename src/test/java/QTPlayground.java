import com.rust.util.MyFrame;
import io.qt.gui.QOpenGLContext;
import io.qt.gui.QOpenGLFunctions;
import io.qt.gui.QSurfaceFormat;
import io.qt.opengl.QOpenGLWindow;
import io.qt.opengl.widgets.QOpenGLWidget;
import io.qt.widgets.QApplication;
import io.qt.widgets.QMessageBox;
import io.qt.widgets.QWidget;

public class QTPlayground extends QOpenGLWidget {
    @Override
    protected void initializeGL() {
        super.initializeGL();
    }

    @Override
    protected void paintGL() {
        super.paintGL();
        QOpenGLContext context = QOpenGLContext.currentContext();
        QOpenGLFunctions f = context.functions();
        System.out.println(1);
    }

    public static void main(String[] args){
        QApplication.initialize(args);
        MyFrame frame = new MyFrame("Hello");
        frame.show();
        QApplication.exec();
    }
}
