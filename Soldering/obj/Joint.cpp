#include "Joint.h"
#include <math.h>


double distance(POINT p1, POINT p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

POINT calculateThirdPoint(POINT A, POINT B, double AB, double BC, double CA) {
	double cosACB = (CA * CA + AB * AB - BC * BC) / (2 * CA * AB);
	double angleACB = acos(cosACB);

	double unitVectorAB_x = (B.x - A.x) / AB;
	double unitVectorAB_y = (B.y - A.y) / AB;

	double unitVectorAC_x = cos(angleACB) * unitVectorAB_x - sin(angleACB) * unitVectorAB_y;
	double unitVectorAC_y = sin(angleACB) * unitVectorAB_x + cos(angleACB) * unitVectorAB_y;

	POINT C;
	C.x = A.x + CA * unitVectorAC_x;
	C.y = A.y + CA * unitVectorAC_y;

	return C;
}

void rotatePoint(double x1, double y1, double x2, double y2, double theta, double* rotatedX, double* rotatedY) {
	double translatedX = x2 - x1;
	double translatedY = y2 - y1;

	double cosTheta = cos(theta);
	double sinTheta = sin(theta);
	double rotatedTranslatedX = translatedX * cosTheta - translatedY * sinTheta;
	double rotatedTranslatedY = translatedX * sinTheta + translatedY * cosTheta;

	*rotatedX = rotatedTranslatedX + x1;
	*rotatedY = rotatedTranslatedY + y1;
}

Joint::Joint() {
	m_bFlip = FALSE;
}

Joint::Joint(POINT org, int length0, int length1) {
	SetParameters(org, length0, length1, FALSE);
}

Joint::~Joint() {

}

void Joint::SetParameters(POINT org, int length0, int length1, BOOL flip) {
	m_posOrg = org;
	m_nLength0 = length0;
	m_nLength1 = length1;

	m_posTarget.x = org.x;
	m_posTarget.y = org.y + (m_nLength0 + m_nLength1);

	m_posMiddle.x = org.x;
	m_posMiddle.y = org.y + m_nLength0;

	m_bFlip = flip;
}

void Joint::GetPositions(POINT& org, POINT& middle, POINT& target) {
	org = m_posOrg;
	middle = m_posMiddle;
	target = m_posTarget;
}

void Joint::TestPaint(HDC hDC, int w, int h) {
	POINT pt;
	MoveToEx(hDC, X(m_posOrg.x, w), Y(m_posOrg.y, h), &pt);
	LineTo(hDC, X(m_posMiddle.x, w), Y(m_posMiddle.y, h));
	LineTo(hDC, X(m_posTarget.x, w), Y(m_posTarget.y, h));

	int radius = 5;
	Ellipse(hDC, X(m_posOrg.x, w) - radius, Y(m_posOrg.y, h) - radius, X(m_posOrg.x, w) + radius, Y(m_posOrg.y, h) + radius);
	Ellipse(hDC, X(m_posMiddle.x, w) - radius, Y(m_posMiddle.y, h) - radius, X(m_posMiddle.x, w) + radius, Y(m_posMiddle.y, h) + radius);
	Ellipse(hDC, X(m_posTarget.x, w) - radius, Y(m_posTarget.y, h) - radius, X(m_posTarget.x, w) + radius, Y(m_posTarget.y, h) + radius);
}

void Joint::SetTarget(POINT target) {
	int d = distance(m_posOrg, target);
	double theta;

	double dx = target.x - m_posOrg.x;
	double dy = target.y - m_posOrg.y;
	if (dx == 0) {
		theta = 3.141592 / 2;
	}
	else {
		theta = atan2(dy, dx);
	}

	m_posTarget.x = m_posOrg.x;
	m_posTarget.y = m_posOrg.y + d;

	int x1 = m_posOrg.x;
	int y1 = m_posOrg.y;

	int x2 = m_posTarget.x;
	int y2 = m_posTarget.y;

	int AB = distance(m_posOrg, m_posTarget);
	int AC = m_nLength0;

	int x3 = x1 + AC * (x2 - x1) / AB;
	int y3 = y1 + AC * (y2 - y1) / AB;

	POINT p = calculateThirdPoint(m_posOrg, m_posTarget, distance(m_posOrg, m_posTarget), m_nLength1, m_nLength0);
	m_posMiddle = p;

	if (m_bFlip) {
		m_posMiddle.x = 2 * m_posOrg.x - m_posMiddle.x;
	}
	double new_x;
	double new_y;
	rotatePoint(m_posOrg.x, m_posOrg.y, m_posMiddle.x, m_posMiddle.y, -(3.141592 / 2 - theta), &new_x, &new_y);
	m_posMiddle.x = new_x;
	m_posMiddle.y = new_y;

	rotatePoint(m_posOrg.x, m_posOrg.y, m_posTarget.x, m_posTarget.y, -(3.141592 / 2 - theta), &new_x, &new_y);
	m_posTarget.x = new_x;
	m_posTarget.y = new_y;
}
