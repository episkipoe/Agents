#include <QTimer>
#include <QCheckBox>
#include <QDockWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include "WorldView.h"
#include "agent.h"
#include "announcements.h"

extern vector <Agent *> agents;

WorldView::WorldView(QWidget *parent) : QGLWidget(parent) {
	setFocusPolicy(Qt::StrongFocus);

	heading = 0.0;  speed = 0.2;
	left = right = up = down = in = out = false;
	label_agents = false;

	QTimer * eye_timer = new QTimer(this);
	connect(eye_timer, SIGNAL(timeout()), this, SLOT(move_eye()));
	eye_timer->start(100);
}

WorldView::~WorldView() {
	makeCurrent();
}

void WorldView::initializeGL() {
	glClearColor(0.0f,0.0f,0.0f,1.0f); 
	glEnable(GL_DEPTH_TEST);
  	glDepthFunc(GL_LESS);
  	glPointSize(4.0);
	glLineWidth(2.0);
	eye.z=100;
}

void WorldView::set_eye(void) {
	//printf("eye is %g,%g,%g looking at %g,%g,%g\n", eye.x, eye.y, eye.z, look.x, look.y, look.z);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, look.x, look.y, look.z, 0.0, 1.0, 0.0);
}

void WorldView::move_eye(void) {
	float delta = log(eye.z+2.0)*speed;
	//printf("move left %i right %i up %i down %i\n", left, right, up, down);
	if(left) {
		eye.x -= delta;
		look.x -= delta;
	}
	if(right) {
		eye.x += delta;
		look.x += delta;
	}
	if(up) {
		eye.y += delta;
		look.y += delta;
	}
	if(down) {
		eye.y -= delta;
		look.y -= delta;
	}
	if(in) {
		eye.z -= delta*2;
		if(eye.z<2) eye.z = 2;
		look.z -= delta*2;
		if(look.z<0) look.z = 0;
	}
	if(out) {
		eye.z += delta*2;
		look.z += delta*2;
	}
	int any=left+right+up+down+in+out;
	if(any) set_eye();
	updateGL();
}

void WorldView::reset_eye(void) {
	eye.x=eye.y=0.0; 
	eye.z=100.0;
	look.x=look.y=look.z=0.0;
	set_eye();
}

void WorldView::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);
	glColor3f(1.0,1.0,1.0); glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	for (unsigned int i=0;i<agents.size();i++) {
		agents[i]->draw();

		if(label_agents) {
			Point * location = agents[i]->get_location();
			if(location) {
				glColor3f(1.0,1.0,1.0);
				renderText(location->x, location->y, 0.0, tr(agents[i]->getName().c_str()));
			}
		}
	}
	draw_announcements();
}

void WorldView::resizeGL(int width, int height) {
   	glMatrixMode(GL_PROJECTION) ;
   	glLoadIdentity();
	gluPerspective(45, width / (double)height, 0.2, 1000);
	set_eye();
	glViewport(0, 0, width, height);
}

void WorldView::mousePressEvent(QMouseEvent *event) {
}

void WorldView::mouseMoveEvent(QMouseEvent *event) {
}

void WorldView::keyPressEvent(QKeyEvent * event) {
	switch(event->key()) {
		case Qt::Key_Left: left = true; break;
		case Qt::Key_Right: right = true; break;
		case Qt::Key_Up: up = true; break;
		case Qt::Key_Down: down = true; break;
		case Qt::Key_PageUp: in = true; break;
		case Qt::Key_PageDown: out = true; break;
		case Qt::Key_Home: reset_eye(); break;
		default: QWidget::keyPressEvent(event); return;
	}
}

void WorldView::keyReleaseEvent(QKeyEvent * event) {
	switch(event->key()) {
		case Qt::Key_Left: left = false; break;
		case Qt::Key_Right: right = false; break;
		case Qt::Key_Up: up = false; break;
		case Qt::Key_Down: down = false; break;
		case Qt::Key_PageUp: in = false; break;
		case Qt::Key_PageDown: out = false; break;
		default: QWidget::keyPressEvent(event); return;
	}
}

void WorldView::DisplayOptions(QMainWindow * parent) {
	QDockWidget *dock = new QDockWidget(tr("Display Options"), parent);
	parent->addDockWidget(Qt::LeftDockWidgetArea, dock);

	QGroupBox *options = new QGroupBox(dock);
	QVBoxLayout * vbox = new QVBoxLayout;
	options->setLayout(vbox);
	dock->setWidget(options);

	QCheckBox * label = new QCheckBox("&Label organisms ", options);
	label->setCheckState(label_agents ? Qt::Checked : Qt::Unchecked);
	vbox->addWidget(label);
	connect(label, SIGNAL(stateChanged(int)), this, SLOT(show_agent_labels(int)));

	vbox->addStretch();
}


