#ifndef WORLD_VIEW_H
#define WORLD_VIEW_H

#include <QMainWindow>
#include <QGLWidget>
#include <QKeyEvent>
#include <common/point.h>

class WorldView: public QGLWidget {
Q_OBJECT
	friend class WorldWindow;
public:
	WorldView(QWidget * parent=0);
	~WorldView();

	void DisplayOptions(QMainWindow * parent);
	
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent * event) ;
	void keyReleaseEvent(QKeyEvent * event) ;

private:
	Point eye;
	Point look;
	float heading, speed;
	bool left, right, up, down, in, out;
	void set_eye();
	void reset_eye(void) ;

	bool label_agents;

public slots:
	void move_eye(void) ;
	void show_agent_labels (int state) { label_agents = (state == Qt::Checked); }
};

#endif

