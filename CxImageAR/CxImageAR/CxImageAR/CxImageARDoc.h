

#pragma once

#include "stdafx.h"

class CPointInt{	
public :
	int x;
	int y;

public :
	CPointInt();
	CPointInt(int x, int y);
};

class CLabel{
public:
	CPointInt p[2];
	int index;
	CLabel();
	void init(int idx, CPointInt p0, CPointInt p1);
};

class CStack{
private :
	CPointInt stack[MAX_ELEMENT];
	int top;

public :
	CStack();
	void push(CPointInt p);
	CPointInt pop();
	BOOL isFull();
	BOOL isEmpty();
};

class CQueue{
private :
	CPointInt queue[MAX_ELEMENT];
	int head;
	int tail;

public :
	CQueue();
	void init();
	void put(CPointInt p);
	CPointInt get();
	BOOL isFull();
	BOOL isEmpty();
};


class CCxImageARDoc{
private :
	CxImage* m_pImage;
	CxImage* m_pRender;
	CxImage* m_pTemplate;
	CxImage* m_pSearch;

	CStack* cstack;
	CQueue* cqueue;
	CLabel* clabel;
	CLabel marker;
	int candidate_marker;

public :
	inline CxImage *GetImage() { return m_pImage; }

private:
	
	CxImage* initImage(int width, int height);
	RGBQUAD setColor(int index);
	void calcLabel(int index, CLabel* clable, CxImage* src);
	void adjSearchStack(int x, int y, int index, CxImage* src, CxImage* dst);
	void adjSearchQueue(int x, int y, int index, CxImage* src, CxImage* dst);
	void visit(int x, int y, CxImage* dst, int index);

	CxImage* Reverse(CxImage* src);
	
	

public:
	CCxImageARDoc();
	int FindType(const CString& ext);
	void drawRect(CPointInt p1, CPointInt p2, CxImage* src, RGBQUAD color);
	//file open 
	BOOL OnOpenImage(LPCTSTR lpszPathName, int idx);
	
	CxImage* AugmentedReality(CxImage* m_pImage);

	CxImage* Grayscale(CxImage* m_pImage);
	CxImage* Binarization(CxImage* m_pImage);
	CxImage* Edgedetection(CxImage* m_pImage);
	CxImage* Labeling(CxImage* m_pImage);
	BOOL templateMatching(CxImage* src, CxImage* dst);
	CxImage* extractRegion(CPointInt p1, CPointInt p2, CxImage* src);

};