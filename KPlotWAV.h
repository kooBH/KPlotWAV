#pragma once

#include <QOpenGLWidget>

class KPlotWAV : public QOpenGLWidget {
	Q_OBJECT

private:

public:

	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* e) override;
};