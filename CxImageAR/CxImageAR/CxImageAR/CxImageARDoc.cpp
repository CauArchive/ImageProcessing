
/* CxImageARDoc.cpp : implement a Augmented Reality */
#include "stdafx.h"
#include "CxImageARDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPointInt::CPointInt(){
	x = -1;
	y = -1;
}

CPointInt::CPointInt(int x1, int y1){
	x = x1;
	y = y1;
}

CStack::CStack(){
	top = 1;
}

void CStack::push(CPointInt p){
	if(!isFull()){
		stack[top] = p;
		top++;
	}
}

CPointInt CStack::pop(){
	if(!isEmpty()){
		top--;
		return stack[top];
	}
	else{
		CPointInt temp;
		return temp;
	}
}

BOOL CStack::isFull(){
	if(top < MAX_ELEMENT)
		return TRUE;
	else 
		return FALSE;
}

BOOL CStack::isEmpty(){
	if(top == 1)
		return TRUE;
	else
		return FALSE;
}

CQueue::CQueue(){
	head = 0;
	tail = 2;
}

void CQueue::init(){
	head = 0;
	tail = 2;
}

void CQueue::put(CPointInt p){
	if(!isFull()){
		queue[tail-2] = p;
		tail = (tail+1)%MAX_ELEMENT;
	}
}

CPointInt CQueue::get(){
	CPointInt temp;
	if(!isEmpty()){
		temp = queue[head];
		head = (head+1)%MAX_ELEMENT;
	}
	return temp;
}

BOOL CQueue::isFull(){
	if(tail-head == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL CQueue::isEmpty(){
	if(tail-head == 2)
		return TRUE;
	else
		return FALSE;
}

CLabel::CLabel(){
	index = 0;
}

void CLabel::init(int idx, CPointInt p0, CPointInt p1){
	index = idx;
	p[0].x = p0.x;
	p[0].y = p0.y;
	p[1].x = p1.x;
	p[1].y = p1.y;
}

CCxImageARDoc::CCxImageARDoc(){
	m_pImage = NULL;
	cstack = new CStack;
	cqueue = new CQueue;
}

int CCxImageARDoc::FindType(const CString& ext)
{
	return CxImage::GetTypeIdFromName(ext);
}

CxImage* CCxImageARDoc::initImage(int width, int height){
	CxImage* temp = new CxImage;
	temp->Create(width, height, 24, CXIMAGE_FORMAT_BMP);

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			RGBQUAD color;
			color.rgbRed = 0;
			color.rgbGreen = 0;
			color.rgbBlue = 0;
			temp->SetPixelColor(x, y, color);
		}
	}
	return temp;
}

RGBQUAD CCxImageARDoc::setColor(int index){
	RGBQUAD color;
	switch(index){
	case 0:
		color.rgbRed	= 0;
		color.rgbGreen	= 0;
		color.rgbBlue	= 0;
		break;

	case 1:
		color.rgbRed	= 255;
		color.rgbGreen	= 0;
		color.rgbBlue	= 0;
		break;
	
	case 2:
		color.rgbRed	= 0;
		color.rgbGreen	= 255;
		color.rgbBlue	= 255;
		break;

	case 3:
		color.rgbRed	= 0;
		color.rgbGreen	= 0;
		color.rgbBlue	= 255;
		break;

	case 4:
		color.rgbRed	= 0;
		color.rgbGreen	= 255;
		color.rgbBlue	= 100;
		break;

	case 5:
		color.rgbRed	= 255;
		color.rgbGreen	= 0;
		color.rgbBlue	= 255;
		break;

	case 6:
		color.rgbRed	= 255;
		color.rgbGreen	= 255;
		color.rgbBlue	= 0;
		break;

	case 255:
		color.rgbRed	= 255;
		color.rgbGreen	= 0;
		color.rgbBlue	= 0;
		break;

	default:
		color.rgbRed	= 255;
		color.rgbGreen	= 255;
		color.rgbBlue	= 255;
		break;
	}
	return color;
}

void CCxImageARDoc::drawRect(CPointInt p1, CPointInt p2, CxImage* src, RGBQUAD color){
	int width = m_pImage->GetWidth();
	int height = m_pImage->GetHeight();
	
	int y1 = (p1.y-2 > 0) ? p1.y-2 : 0;
	int y2 = (p2.y+2 < height) ? p2.y+2 : height;
	int x1 = (p1.x-2 > 0) ? p1.x-2 :0;
	int x2 = (p2.x+2 < width) ? p2.x+2 : width;

	for(int y = y1; y < y2; y++){
		for(int x = x1; x < x2; x++){
			if((((y > p1.y+2) || (y < p2.y-2)) && ((x < p1.x+2) || (x > p2.x-2))) ||
				(y < p1.y+2) || (y > p2.y-2)){
				src->SetPixelColor(x, y, color);
			}
		}
	}	
}



void CCxImageARDoc::calcLabel(int index, CLabel* clable, CxImage* src){
	int width = src->GetWidth();
	int height = src->GetHeight();

	
	//initialize of label
	CLabel* temp_label = new CLabel[index+1];
	clabel = temp_label;
	for(int i = 0; i < index+1; i++){
		clabel[i].init(i, CPointInt(width, height), CPointInt(0, 0));
	}
	
	RGBQUAD color;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			color = src->GetPixelColor(x, y);
			int idx;
			if(color.rgbRed != 0){
				idx = color.rgbRed;
				if(clabel[idx].p[0].x > x)
					clabel[idx].p[0].x = x;
				if(clabel[idx].p[0].y > y)
					clabel[idx].p[0].y = y;
				if(clabel[idx].p[1].x < x)
					clabel[idx].p[1].x = x;
				if(clabel[idx].p[1].y < y)
					clabel[idx].p[1].y = y;
			}
		}
	}
}

void CCxImageARDoc::visit(int x, int y, CxImage* dst, int index){
	RGBQUAD dstcolor;
	dstcolor.rgbRed = index;
	dstcolor.rgbGreen = 0;
	dstcolor.rgbBlue = 0;
	dst->SetPixelColor(x, y, dstcolor);
}

CxImage* CCxImageARDoc::Reverse(CxImage* src){
	int width = src->GetWidth();
	int height = src->GetHeight();
	
	CxImage* dst = new CxImage;
	dst->Create(width, height, 24, CXIMAGE_FORMAT_BMP);

	RGBQUAD color, newcolor;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			color = src->GetPixelColor(x, y);
			if(color.rgbRed == 255){
				newcolor.rgbRed = 0;
				newcolor.rgbGreen = 0;
				newcolor.rgbBlue = 0;
			}
			else{
				newcolor.rgbRed = 255;
				newcolor.rgbGreen = 255;
				newcolor.rgbBlue = 255;
			}
			dst->SetPixelColor(x, y, newcolor);
		}
	}
	return dst;
}


/* File Open */
BOOL CCxImageARDoc::OnOpenImage(LPCTSTR lpszPathName, int idx){
	if(idx == 0){
		m_pImage = new CxImage;
		m_pImage->Load(lpszPathName, FindType(lpszPathName));
	}
	else{
		m_pRender = new CxImage;
		m_pRender->Load(lpszPathName, FindType(lpszPathName));
	}
	return TRUE;
}


CxImage* CCxImageARDoc::AugmentedReality(CxImage* m_pImage){
	int width = m_pImage->GetWidth();
	int height = m_pImage->GetHeight();

	CxImage* buffer = Labeling(m_pImage);
	CxImage* dst = new CxImage;
	dst->Copy(*m_pImage);
	
	CxImage* cropImage = new CxImage;
	CxImage* templateImage = new CxImage;
	TCHAR* templatePath[4];
	templatePath[0] = _T("..\\template\\template01.bmp");
	templatePath[1] = _T("..\\template\\template02.bmp");
	templatePath[2] = _T("..\\template\\template03.bmp");
	templatePath[3] = _T("..\\template\\template04.bmp");
	
	int degree = -1;
	buffer = Binarization(m_pImage);
	//buffer = Reverse(buffer);

	for(int i = 1; i < candidate_marker; i++){
		for(int k = 0; k < 4; k++){
			templateImage->Load(templatePath[k],FindType(templatePath[k]));
			templateImage = Binarization(templateImage);
			cropImage = extractRegion(clabel[i].p[0], clabel[i].p[1], buffer);
			cropImage->Resample(64, 64);

			if(templateMatching(templateImage, cropImage)){
				marker = clabel[i];
				degree = k;
				break;
			}			
		}
		if(degree != -1)
			break;
	}

	m_pRender->Resample(marker.p[1].x - marker.p[0].x, marker.p[1].y - marker.p[0].y);
	
	if(degree != -1){
		switch(degree){
		case 1:
			m_pRender->RotateLeft(m_pRender);
			break;
		case 2:
			m_pRender->RotateRight(m_pRender);
			break;
		case 3:
			m_pRender->Rotate180(m_pRender);
			break;
		default:
			break;
		}
		dst->MixFrom(*m_pRender, marker.p[0].x, marker.p[0].y);
	}
	delete buffer;
	delete cropImage;
	delete templateImage;
	return dst;
}


CxImage* CCxImageARDoc::Grayscale(CxImage* m_pImage){

	int width = m_pImage->GetWidth();
	int height = m_pImage->GetHeight();

	CxImage* dst = new CxImage;
	dst->Create(width, height, 24, CXIMAGE_FORMAT_BMP);

	RGBQUAD color;
	RGBQUAD newcolor;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			color = m_pImage->GetPixelColor(x, y);
			
			newcolor.rgbBlue = (color.rgbBlue + color.rgbGreen + color.rgbRed)/3;
			newcolor.rgbGreen = (color.rgbBlue + color.rgbGreen + color.rgbRed)/3;
			newcolor.rgbRed = (color.rgbBlue + color.rgbGreen + color.rgbRed)/3;

			dst->SetPixelColor(x, y, newcolor);
		}
	}

	return dst;
}

CxImage* CCxImageARDoc::Binarization(CxImage* m_pImage){

	int width = m_pImage->GetWidth();
	int height = m_pImage->GetHeight();

	CxImage* dst = Grayscale(m_pImage);

	RGBQUAD color;
	RGBQUAD newcolor;
	int threshold = 128;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			color = dst->GetPixelColor(x, y);
			
			if(threshold > color.rgbBlue){
				newcolor.rgbBlue = 0;
				newcolor.rgbGreen = 0;
				newcolor.rgbRed = 0;
			}
			else{
				newcolor.rgbBlue = 255;
				newcolor.rgbGreen = 255;
				newcolor.rgbRed = 255;
			}
			dst->SetPixelColor(x, y, newcolor);
		}
	}
	return dst;
}

CxImage* CCxImageARDoc::Edgedetection(CxImage* m_pImage){
	int width = m_pImage->GetWidth();
	int height = m_pImage->GetHeight();

	CxImage* dst = Binarization(Grayscale(m_pImage));

	CxImage* buffer = new CxImage;
	buffer->Create(width, height, 24, CXIMAGE_FORMAT_BMP);

	RGBQUAD color;
	RGBQUAD newcolor;

	int mask_a[3][3] = {{1, 2, 1},{0, 0, 0},{-1, -2, -1}};
	int mask_b[3][3] = {{1, 0, -1},{2, 0, -2},{1, 0, -1}};
	int mask_c[3][3] = {{-2, -1, 0},{-1, 0, 1},{0, 1, 2}};
	int mask_d[3][3] = {{0, 1, 2},{-1, 0, 1},{-2, -1, 0}};

	int threshold = 510;

	for(int y = 0; y < height -2; y++){
		for(int x = 0; x < width -2; x++){

			int sum = 0;
			for(int j = 0; j < 3; j++){
				for(int i = 0 ; i < 3; i++){
					color = dst->GetPixelColor(x+i, y+j);
					sum += color.rgbRed * mask_c[i][j];
				}
			}

			if((sum > threshold) || (sum < -threshold)){
				newcolor.rgbRed = 255;
				newcolor.rgbGreen = 255;
				newcolor.rgbBlue = 255;
				buffer->SetPixelColor(x+1, y+1, newcolor);
			}
			else{
				newcolor.rgbRed = 0;
				newcolor.rgbGreen = 0;
				newcolor.rgbBlue = 0;
				buffer->SetPixelColor(x+1, y+1, newcolor);
			}
		}
	}

	for(int y = 0; y < height -2; y++){
		for(int x = 0; x < width -2; x++){

			int sum = 0;
			for(int j = 0; j < 3; j++){
				for(int i = 0 ; i < 3; i++){
					color = dst->GetPixelColor(x+i, y+j);
					sum += color.rgbRed * mask_d[i][j];
				}
			}

			if((sum > threshold) || (sum < -threshold)){
				newcolor.rgbRed = 255;
				newcolor.rgbGreen = 255;
				newcolor.rgbBlue = 255;
				buffer->SetPixelColor(x+1, y+1, newcolor);
			}			
		}
	}

	for(int y = 1; y < height -1; y++){
		for(int x = 1; x < width -1; x++){
			color = buffer->GetPixelColor(x, y);
			dst->SetPixelColor(x, y, color);
		}
	}

	delete buffer;
	return dst;
}

CxImage* CCxImageARDoc::Labeling(CxImage* m_pImage){
	int width = m_pImage->GetWidth();
	int height = m_pImage->GetHeight();

	CxImage* src = Binarization(Grayscale(m_pImage));
	CxImage* dst = initImage(width, height);	//dst is buffer

	RGBQUAD color, dstcolor;
	int index = 1;
	
	for(int y = 0; y < height-1; y++){
		for(int x = 0; x < width-1; x++){
			/* you implement this */
		}
	}

	candidate_marker = index;

	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			color = dst->GetPixelColor(x, y);
			dst->SetPixelColor(x, y, setColor(color.rgbRed));

		}
	}

	return dst;
}

void CCxImageARDoc::adjSearchStack(int x, int y, int index, CxImage* src, CxImage* dst){
	int width = src->GetWidth();
	int height = src->GetHeight();
	
	/* search the adj pixel using stack */

}


void CCxImageARDoc::adjSearchQueue(int x, int y, int index, CxImage* src, CxImage* dst){
	int width = src->GetWidth();
	int height = src->GetHeight();
	
	/* search the adj pixel using queue */
}


BOOL CCxImageARDoc::templateMatching(CxImage* src, CxImage* dst){
	int width = src->GetWidth();
	int height = src->GetHeight();

	RGBQUAD srccolor, dstcolor;
	double total = width * height;
	int correct = 0;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			/* you implement this */
		}
	}
	if((double)correct/total > 0.85){
		return TRUE;
	}
	else{
		return FALSE;
	}
}



CxImage* CCxImageARDoc::extractRegion(CPointInt p1, CPointInt p2, CxImage* src){
	int width = p2.x - p1.x;
	int height = p2.y - p1.y;
	
	CxImage* dst = initImage(width, height);
	
	RGBQUAD color, newcolor;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			color = src->GetPixelColor(x + p1.x, y + p1.y);
			newcolor.rgbRed = color.rgbRed;
			newcolor.rgbGreen = color.rgbGreen;
			newcolor.rgbBlue = color.rgbBlue;

			dst->SetPixelColor(x, y, newcolor);
		}
	}

	return dst;
}
