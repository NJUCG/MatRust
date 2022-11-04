package com.rust.view;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.util.Semantic;
import com.rust.viewModel.ControllPanelViewModel;
import com.rust.viewModel.ControllPanelViewModelDelegate;
import com.rust.viewModel.ControllableArgument;

import java.awt.*;
import java.nio.IntBuffer;
import static org.lwjgl.opengl.GL33C.*;
public class ControllPanel implements ControllPanelViewModelDelegate {
    Frame frame;

    private final static int panelHeight = 400;
    private final static int panelWidth = 300;
    private final static int barHeight = 30;
    private final static int labelHeight = 30;
    private final static int labelWidth = 60;
    private final static int labelInset = 20;
    private final static int labelBarSpace = 20;
    private final static int barWidth = 150;
    private final static int barSpace = 10;
    private final static int startBtnWidth = 100;
    private final static int startBtnHeight = 50;
    private int currentHeight = 40;
    private final ControllPanelViewModel viewModel;

    public ControllPanel(){
        frame = new Frame();
        frame.setSize(panelWidth,panelHeight);
        frame.setTitle("控制面板");
        frame.setLayout(null);
        frame.setResizable(false);
        frame.setVisible(true);

        viewModel = ControllPanelViewModel.shared;
        viewModel.setUp(this);

        Button startBtn = new Button();
        startBtn.setBounds(panelWidth/2 - startBtnWidth/2,currentHeight,startBtnWidth,startBtnHeight);
        startBtn.addActionListener(viewModel);
        startBtn.setLabel("Start");
        frame.add(startBtn);
        currentHeight += startBtnHeight + barSpace;
        Button clearBtn = new Button();
        clearBtn.setBounds(panelWidth/2 - startBtnWidth/2,currentHeight,startBtnWidth,startBtnHeight);
        clearBtn.addActionListener(viewModel);
        clearBtn.setLabel("Clear");
        frame.add(clearBtn);
        currentHeight += startBtnHeight + barSpace;
    }

    //现在参数铺到到高度


    @Override
    public void addArgument(ControllableArgument arg) {
        String argString = viewModel.getArgumentName(arg);
        Label label = new Label(argString);
        Scrollbar scrollbar = new Scrollbar(Scrollbar.HORIZONTAL);
        scrollbar.addAdjustmentListener(viewModel.getAdjustListener(arg));

        label.setBounds(labelInset,currentHeight,labelWidth,labelHeight);
        scrollbar.setBounds(labelInset + labelWidth + labelBarSpace,currentHeight,barWidth,barHeight);

        currentHeight = currentHeight + barHeight + barSpace;

        frame.add(label);
        frame.add(scrollbar);
    }
}
