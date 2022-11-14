package com.rust.view;

import com.rust.util.MyFrame;
import com.rust.view.window.NodeObject;
import com.rust.view.window.NodeSlot;
import io.qt.widgets.QApplication;

import java.io.FileNotFoundException;

public class TestConfig {
    public static void main(String[] args) throws FileNotFoundException {
        NodeObject.init();
        NodeSlot.init();
        try {
            QApplication.initialize(args);
            MyFrame frame = new MyFrame();
            frame.show();
            QApplication.exec();
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
