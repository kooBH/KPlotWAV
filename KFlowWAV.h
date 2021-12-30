#pragma once

#include <QOpenGLWidget>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>

class KFlowWAV : public QOpenGLWidget {
	Q_OBJECT

private:
	int height = 128;
	int gap_btw_point = 2;
	int samples_in_point = 100;
	int shift = 128;
	short* buf;
	int idx = 0;


	int n_frame = 0;
	int x=1;
	int y=0;
	int y_prev = 0;
	int total_sample = 0;

  QPen pen;
  QPen pen2;
	QImage img;
	QPixmap pic,tmp_pic;
	QPainter painter;

	void expand_buf(int shift);

public:
	KFlowWAV();
	~KFlowWAV();


	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* e) override;

public slots :
	void plot(short*, int n_sample);


};