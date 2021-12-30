#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <QFileDialog>
#include "WAV.h"
#include "KPlotWAV.h"
#include "KFlowWAV.h"

#include <QScrollBar>

#include <thread>

class KWidgetPlotWAV : public QWidget {
    Q_OBJECT

private :
  QVBoxLayout layout_main;

  QHBoxLayout layout_control;
  QPushButton btn_load;
  QLabel label_path;
  QPushButton btn_smaller;
  QPushButton btn_bigger;
  
  QScrollArea area_plot;
  QWidget widget_plot;
  QVBoxLayout layout_plot;
  KFlowWAV plot_1;
  KFlowWAV plot_2;

  int shift = 128;

  std::thread* thread_load=nullptr;


public :

   KWidgetPlotWAV();

   void load(QString);


signals :
  void signal_load(QString path);
  void signal_plot_1(short*, int);
  void signal_plot_2(short*, int);

public slots:
  void slot_load(QString path);

};