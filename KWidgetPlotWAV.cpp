#include "KWidgetPlotWAV.h"

KWidgetPlotWAV::KWidgetPlotWAV() {
  btn_load.setText("Load");
  label_path.setText("path");
  btn_smaller.setText("Smaller");
  btn_bigger.setText("Bigger");

  layout_control.addWidget(&btn_load);
  layout_control.addWidget(&label_path);
  layout_control.addWidget(&btn_smaller);
  layout_control.addWidget(&btn_bigger);
  layout_main.addLayout(&layout_control);

  layout_plot.addWidget(&plot_1);
  layout_plot.addWidget(&plot_2);
  widget_plot.setLayout(&layout_plot);

  layout_main.addWidget(&area_plot);
  area_plot.setWidget(&widget_plot);

  setLayout(&layout_main);

  layout_plot.setAlignment(Qt::AlignTop);

  // Essential
  area_plot.setWidgetResizable(true);   

  this->setFixedSize(1700, 640);

  QObject::connect(&btn_load, &QPushButton::clicked, [&]() {
    QString fileName;
    QFileDialog dialog;

    fileName = dialog.getOpenFileName(this,
      tr("Open Wav File"), ".", tr("something (*.wav)"));
    // Exception
    if (fileName.isEmpty())
      return;
   // emit(signal_load(fileName));
    //load(fileName);

    if (thread_load)delete thread_load;
    thread_load = new std::thread(&KWidgetPlotWAV::load, this,fileName);
    thread_load->detach();
    });

  QObject::connect(this, &KWidgetPlotWAV::signal_load, this,&KWidgetPlotWAV
  ::slot_load);

  QObject::connect(&btn_bigger, &QPushButton::clicked, [&]() {
    //plot_1.plot(nullptr); 

    // Move to Rightest
    area_plot.horizontalScrollBar()->setSliderPosition(area_plot.horizontalScrollBar()->maximum());
  });

  QObject::connect(this, &KWidgetPlotWAV::signal_plot_1, &plot_1, &KFlowWAV::plot);
  QObject::connect(this, &KWidgetPlotWAV::signal_plot_2, &plot_2, &KFlowWAV::plot);
}

void KWidgetPlotWAV::load(QString path) {
  printf("void KWidgetPlotWAV::load(QString path)\n");
  WAV wav;

  int shift = 128;

  wav.OpenFile(path.toStdString());

  short *buf, *buf_1,*buf_2;
  buf = new short[shift * 2];
  buf_1 = new short[shift];
  buf_2 = new short[shift];

  int cnt = 0;
  
  while (!wav.IsEOF()) {
    wav.ReadUnit(buf, shift * 2);

    for (int i = 0; i < shift; i++) {
      buf_1[i] = buf[2 * i];
      buf_2[i] = buf[2 * i+1];
    } 
    
   // plot_1.plot(buf_1,shift);
   // plot_2.plot(buf_2,shift);
    emit(signal_plot_1(buf_1, shift));
    emit(signal_plot_2(buf_2, shift));
    // Move to Rightest
    area_plot.horizontalScrollBar()->setSliderPosition(area_plot.horizontalScrollBar()->maximum());

    cnt++;
  //  printf("cnt : %d\n", cnt);
  //  if (cnt > 10)break;
    Sleep(8);
   // update();
    
  }

  delete[] buf;
  delete[] buf_1;
  delete[] buf_2;
  wav.Finish();
  printf("Done\n");

 // emit(signal_load(path));
}

void KWidgetPlotWAV::slot_load(QString path) {
  printf("void KWidgetPlotWAV::slot_load(QString path)\n");
  WAV wav;

  int shift = 128;

  wav.OpenFile(path.toStdString());

  short *buf, *buf_1,*buf_2;
  buf = new short[shift * 2];
  buf_1 = new short[shift];
  buf_2 = new short[shift];

  int cnt = 0;
  
  while (!wav.IsEOF()) {
    wav.ReadUnit(buf, shift * 2);

    for (int i = 0; i < shift; i++) {
      buf_1[i] = buf[2 * i];
      buf_2[i] = buf[2 * i+1];
    } 
    
    plot_1.plot(buf_1,shift);
    plot_2.plot(buf_2,shift);
    // Move to Rightest
    area_plot.horizontalScrollBar()->setSliderPosition(area_plot.horizontalScrollBar()->maximum());

    cnt++;
    //printf("cnt : %d\n", cnt);
    Sleep(32);
   // update();
    
  }

  update();

  delete[] buf;
  delete[] buf_1;
  delete[] buf_2;
  wav.Finish();
  printf("Done\n");
}
