#include "KFlowWAV.h"

KFlowWAV::KFlowWAV():QOpenGLWidget(){
	setFixedHeight(height);
	setFixedWidth(1600);

//	setAutoFillBackground(false);
	pen.setColor(QColor("blue"));
	pen.setWidth(1);
	pen.setJoinStyle(Qt::RoundJoin);

	pen2.setColor(QColor("black"));
	pen2.setWidth(1);
	pen2.setJoinStyle(Qt::RoundJoin);

	pic = QPixmap(1600, height);
	pic.fill(QColor("gray"));

	buf = new short[shift * 2];
	memset(buf, 0, sizeof(short) * (shift * 2));
}

KFlowWAV::~KFlowWAV() {
	delete[] buf;
}

void KFlowWAV::plot(short* data, int n_sample) {
	expand_buf(n_sample);
	memcpy(&buf[idx], data, sizeof(short) * n_sample);

	total_sample += n_sample;
	int point2display = total_sample / samples_in_point;

	// Draw
	tmp_pic = QPixmap(pic.width(), pic.height());
	tmp_pic.fill(QColor("gray"));

	painter.begin(&tmp_pic);
	painter.setPen(pen);
	pen.setColor(QColor("blue"));
	painter.drawPixmap(QPoint(-point2display*gap_btw_point, 0), pic);
//	painter.setRenderHint(QPainter::Antialiasing);
	x = pic.width() - point2display * gap_btw_point;
	for (int i = 0; i < point2display; i++) {
		short max = 0;
		bool flag_pos = true;

		// get envelope
		for (int j = 0; j < samples_in_point; j++) {
			if (max < std::abs(buf[idx])) {
				max = std::abs(buf[idx]);
				if (buf[idx] > 0)
					flag_pos = true;
				else
					flag_pos = false;
			}
			idx += 1;
		}
		// draw line
		if(flag_pos)
			y = 64 + (double(max) / 32767)*64;
		else 
			y = 64 - (double(max) / 32767)*64;
	//	printf("draw line : %d %d %d %d\n",x,y_prev,x+gap_btw_point,y);
		painter.drawLine(x, y_prev, x + gap_btw_point, y);
		x += gap_btw_point;

		y_prev = y;
	}
  painter.end();

	painter.begin(&tmp_pic);
	painter.setPen(pen2);
	painter.drawLine(0, 64, 1600, 64);
  painter.end();

  pic = tmp_pic;
	// Shift buffer
	for (int i = 0; i < 2 * shift - point2display * samples_in_point; i++) {
		buf[i] = buf[i + point2display * samples_in_point];
	}
	idx -= point2display * samples_in_point;
	total_sample -= point2display * samples_in_point;

	update();
}


void KFlowWAV::expand_buf(int shift_) {
	if (shift_ <= shift)
		return;
	short* t_buf = new short[shift_ * 2];
	memset(t_buf, 0, sizeof(short) * (2*shift_));
	memcpy(t_buf, buf, sizeof(short) * (2 * shift));
	short* t_p = buf;
	buf = t_buf;
	delete[] t_p;

	shift = shift_;

}

// ref : https://github.com/audacity/audacity/blob/7b0b15c123c6aa09a06356f7ed0decf429c68b02/src/tracks/playabletrack/wavetrack/ui/WaveformView.cpp
void KFlowWAV::paintEvent(QPaintEvent* event) {
	painter.begin(this);
	painter.drawPixmap(0,0,pic.width(),pic.height(),pic);
	painter.end();
}


void KFlowWAV::resizeEvent(QResizeEvent* e) {
	painter.begin(this);
	painter.drawPixmap(0,0,pic.width(),pic.height(),pic);
	painter.end();
}
