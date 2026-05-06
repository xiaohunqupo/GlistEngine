/*
 * gApp.cpp
 *
 *  Created on: May 6, 2020
 *      Author: Noyan Culum
 */

#include "GameCanvas.h"
#include "gApp.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
}

void gApp::setup() {
	GameCanvas* cnv = new GameCanvas(this);
	appmanager->setCurrentCanvas(cnv);
}

void gApp::update() {
}
