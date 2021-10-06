
// ImageProcessingDoc.cpp : Implement a CImageProcessingDoc Class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "DlgBrightnessOption.h"
#include "DlgMosaicOption.h"
#include "DlgCompositeOption.h"
#include "DlgIntensityTransformOption.h"
#include "DlgContrastStretchOption.h"

#include "ImageProcessingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessingDoc

IMPLEMENT_DYNCREATE(CImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessingDoc, CDocument)
	ON_COMMAND(ID_PROCESS_MOSAIC, &CImageProcessingDoc::OnProcessMosaic)
	ON_COMMAND(ID_PROCESS_BRIGHTNESS, &CImageProcessingDoc::OnProcessBrightness)
	ON_COMMAND(ID_PROCESS_COMPOSITE, &CImageProcessingDoc::OnProcessComposite)
	ON_COMMAND(ID_PROCESS_INTENSITY_TRANSFORM, &CImageProcessingDoc::OnProcessIntensityTransform)
	ON_COMMAND(ID_PROCESS_CONTRAST_STRETCH, &CImageProcessingDoc::OnProcessContrastStretch)
	ON_COMMAND(ID_PROCESS_EQUALIZATION, &CImageProcessingDoc::OnProcessEqualization)
	ON_COMMAND(ID_32783, &CImageProcessingDoc::OnHistogramSpecification)
END_MESSAGE_MAP()


// CImageProcessingDoc Contruction/Destuction

CImageProcessingDoc::CImageProcessingDoc()
{
	//// TODO: Add an one-time generating code here
	m_pImage = NULL;
}

CImageProcessingDoc::~CImageProcessingDoc()
{
	if (NULL != m_pImage)
		delete m_pImage;
}

BOOL CImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: load imagefile // DONE
	m_pImage = new CxImage;
	m_pImage->Load(lpszPathName, FindType(lpszPathName));

	CalculateHistogram();

	return TRUE;
}

BOOL CImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//// TODO: Add a re-initialization code here
	//// SDI documents will reuse this article

	return TRUE;
}




// CImageProcessingDoc serialization

void CImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//// TODO: Add a saving code here
	}
	else
	{
		//// TODO: Add a loading code here
	}
}


// CImageProcessingDoc diagnosis

#ifdef _DEBUG
void CImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProcessingDoc command

CString CImageProcessingDoc::FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString(_T(""));
}

CString CImageProcessingDoc::RemoveExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(0,i);
		}
	}
	return name;
}

int CImageProcessingDoc::FindType(const CString& ext)
{
	return CxImage::GetTypeIdFromName(ext);
}

void CImageProcessingDoc::CalculateHistogram()
{
	// TODO: Add a calculating histogram code here
	if (m_pImage) {
		// Histogram function, which is implemented in Cximage
		 //m_histogramMax = m_pImage->Histogram(m_histogramRed, m_histogramGreen, m_histogramBlue, m_histogramGray);

		 
		// 가짜코드
		for (int i = 0; i < 256; i++){
			m_histogramRed[i]   = rand() % 200;
			m_histogramGreen[i] = rand() % 200;
			m_histogramBlue[i]  = rand() % 200;
			m_histogramGray[i]  = rand() % 200;
		}

		m_histogramMax = 0;
		for (int i = 0; i < 256; i++){
			m_histogramMax = max(m_histogramMax, m_histogramRed[i]  );
			m_histogramMax = max(m_histogramMax, m_histogramGreen[i]);
			m_histogramMax = max(m_histogramMax, m_histogramBlue[i] );
			m_histogramMax = max(m_histogramMax, m_histogramGray[i] );
		}
		//////////////////////////////////////////////////////////////
	}
}

void CImageProcessingDoc::OnProcessBrightness()
{
	// TODO: Add a changing the brightness histogram code here
	if (m_pImage) {
		DlgBrightnessOption dlg;
		//char str[256];
		//memset(str, NULL, sizeof(str));

		if (dlg.DoModal() == IDOK) {
			// write your own code
			// for applying your effect, you must use m_pImage
			// this code is a simple example for manufacturing image : grayscaling
			// Plus Sig == 0, Minus Sig == 1
			int nPlusMinus = dlg.m_nPlusMinus;
			BYTE byModifyValue = dlg.m_byModifyValue;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;

			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					color = m_pImage->GetPixelColor(x, y);
					//sprintf_s(str, "%d\n", nPlusMinus);
					//OutputDebugStringA(str);
					if (nPlusMinus == 0) {
						if (color.rgbBlue + byModifyValue >= 255) newcolor.rgbBlue = 255;
						else newcolor.rgbBlue = color.rgbBlue + byModifyValue;
						if (color.rgbGreen + byModifyValue >= 255) newcolor.rgbGreen = 255;
						else newcolor.rgbGreen = color.rgbGreen + byModifyValue;
						if (color.rgbRed + byModifyValue >= 255) newcolor.rgbRed = 255;
						else newcolor.rgbRed = color.rgbRed + byModifyValue;
					}
					else {
						if (color.rgbBlue - byModifyValue <= 0) newcolor.rgbBlue = 0;
						else newcolor.rgbBlue = color.rgbBlue - byModifyValue;
						if (color.rgbGreen - byModifyValue <= 0) newcolor.rgbGreen = 0;
						else newcolor.rgbGreen = color.rgbGreen - byModifyValue;
						if (color.rgbRed - byModifyValue <= 0) newcolor.rgbRed = 0;
						else newcolor.rgbRed = color.rgbRed - byModifyValue;
					}
					m_pImage->SetPixelColor(x, y, newcolor);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessMosaic()
{
	// TODO: Add a mosaic code here
	if (m_pImage) {
		DlgMosaicOption dlg;
		char str[256];
		memset(str, NULL, sizeof(str));
		if (dlg.DoModal() == IDOK) {
			DWORD dwWindowSize = dlg.m_dwWindowSize;
			DWORD sqrtDwWindowSize = dwWindowSize * dwWindowSize;
			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;
			DWORD redColorSum = 0;
			DWORD greenColorSum = 0;
			DWORD blueColorSum = 0;

			
			CxImage* buffer = new CxImage;

			if (dwWindowSize == 0) {
				// do blurring
				float mask[3][3] = {};
				buffer->Create(width, height, 24, CXIMAGE_FORMAT_BMP);
				buffer->Copy(*m_pImage);
				for (DWORD y = 0; y < height; y++) {
					for (DWORD x = 0; x < width; x++) {
						float sumRed = 0, sumGreen = 0, sumBlue = 0;
						for (int j = 0; j < 3; j++)
							for (int i = 0; i < 3; i++) {
								color = buffer->GetPixelColor(x + i, y + j);
								sumRed += color.rgbRed * mask[j][i];
								sumGreen += color.rgbGreen * mask[j][i];
								sumBlue += color.rgbBlue * mask[j][i];
							}
						newcolor.rgbRed = sumRed;
						newcolor.rgbGreen = sumGreen;
						newcolor.rgbBlue = sumBlue;
						m_pImage->SetPixelColor(x + 1, y + 1, newcolor);
					}
				}
			}
			else if (dwWindowSize == 1) {
				// do Sharpening
				// float mask[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0} };
				float mask[3][3] = { {-1,-1,-1},{-1,9,-1},{-1,-1,-1} };
				buffer->Create(width, height, 24, CXIMAGE_FORMAT_BMP);
				buffer->Copy(*m_pImage);
				for (DWORD y = 0; y < height; y++) {
					for (DWORD x = 0; x < width; x++) {
						int sumRed = 0, sumGreen = 0, sumBlue = 0;
						for (int j = 0; j < 3; j++)
							for (int i = 0; i < 3; i++) {
								color = buffer->GetPixelColor(x + i, y + j);
								sumRed += color.rgbRed * mask[j][i];
								sumGreen += color.rgbGreen * mask[j][i];
								sumBlue += color.rgbBlue * mask[j][i];
							}
						if (sumRed > 255) sumRed = 255;
						if (sumRed < 0) sumRed = 0;
						if (sumGreen > 255) sumGreen = 255;
						if (sumGreen < 0) sumGreen = 0;
						if (sumBlue > 255) sumBlue = 255;
						if (sumBlue < 0) sumBlue = 0;
						newcolor.rgbRed = sumRed;
						newcolor.rgbGreen = sumGreen;
						newcolor.rgbBlue = sumBlue;
						m_pImage->SetPixelColor(x + 1, y + 1, newcolor);
					}
				}
			}
			else {
				// do Mosaic
				DWORD pixelXRest = width % dwWindowSize;
				DWORD pixelYRest = height % dwWindowSize;
				for (DWORD y = 0; y < height / dwWindowSize + 1; y++) {
					DWORD widthLocation;
					DWORD heightLocation;
					DWORD xOffset;
					DWORD yOffset;
					if (y == height / dwWindowSize + 1)
						if (pixelYRest != 0) {
							yOffset = pixelYRest;
							heightLocation = height / dwWindowSize;
						}
						else
							break;
					else {
						yOffset = dwWindowSize;
						heightLocation = y * dwWindowSize;
					}
					for (DWORD x = 0; x < width / dwWindowSize + 1; x++) {
						redColorSum = 0;
						greenColorSum = 0;
						blueColorSum = 0;

						if (x == width / dwWindowSize + 1)
							if (pixelXRest != 0) {
								xOffset = pixelXRest;
								widthLocation = width / dwWindowSize;
							}
							else
								break;
						else {
							xOffset = dwWindowSize;
							widthLocation = x * dwWindowSize;
						}
						for (DWORD j = heightLocation; j < heightLocation + yOffset; j++) {
							for (DWORD i = widthLocation; i < widthLocation + xOffset; i++) {
								color = m_pImage->GetPixelColor(i, j);
								redColorSum += color.rgbRed;
								greenColorSum += color.rgbGreen;
								blueColorSum += color.rgbBlue;
							}
						}
						newcolor.rgbRed = redColorSum / sqrtDwWindowSize;
						newcolor.rgbGreen = greenColorSum / sqrtDwWindowSize;
						newcolor.rgbBlue = blueColorSum / sqrtDwWindowSize;
						for (DWORD j = heightLocation; j < heightLocation + yOffset; j++)
							for (DWORD i = widthLocation; i < widthLocation + xOffset; i++)
								m_pImage->SetPixelColor(i, j, newcolor);
					};
				}
			}			
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessComposite()
{
	// TODO: Add a composite code here
	if (m_pImage) {
		DlgCompositeOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nOperatorID = dlg.GetCompositeOperatorID();
			CxImage * pSecondImage = dlg.GetSecondImage();

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD firstColor;
			RGBQUAD secondColor;
			RGBQUAD newColor;

			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					firstColor = m_pImage->GetPixelColor(x, y);
					secondColor = pSecondImage->GetPixelColor(x, y);

					newColor.rgbBlue  = (BYTE)RGB2GRAY(secondColor.rgbRed, secondColor.rgbGreen, secondColor.rgbBlue);
					newColor.rgbGreen = (BYTE)RGB2GRAY(secondColor.rgbRed, secondColor.rgbGreen, secondColor.rgbBlue);
					newColor.rgbRed   = (BYTE)RGB2GRAY(secondColor.rgbRed, secondColor.rgbGreen, secondColor.rgbBlue);

					m_pImage->SetPixelColor(x, y, newColor);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessIntensityTransform()
{
	// TODO: Add a Contrast conversion code here
	if (m_pImage) {
		DlgIntensityTransformOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nTransformType = dlg.m_nTransformType;
			double nLinearTransfromCoefficient_a = dlg.m_nLinearTransfromCoefficient_a;
			double nLinearTransfromCoefficient_b = dlg.m_nLinearTransfromCoefficient_b;

			int byThresholdValue = dlg.m_byThresholdValue;

			BYTE byContrastStretching_Start = dlg.m_byContrastStretching_Start;
			BYTE byContrastStretching_End = dlg.m_byContrastStretching_End;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessContrastStretch()
{
	// TODO: Add a Contrast stretching code here
	if (m_pImage) {
		DlgContrastStretchOption dlg;

		if (dlg.DoModal() == IDOK) {
			int nStretchType = dlg.m_nStretchType;
			float fLow = dlg.m_fLow;
			float fHigh = dlg.m_fHigh;

			DWORD width = m_pImage->GetWidth();
			DWORD height = m_pImage->GetHeight();
			RGBQUAD color;
			RGBQUAD newcolor;

			BYTE LUT[256];

			// initialize thresholds
			int nLowTherhs = 0;
			int nHighThresh = 255;


			// compute thresholds
			if (nStretchType == 0) { // Auto

			} else if (nStretchType == 1) { // Ends-In

			}


			// compute LUT


			// trnasfer image
			for (DWORD y = 0; y < height; y++) {
				for (DWORD x = 0; x < width; x++) {
					color = m_pImage->GetPixelColor(x, y);

					// using LUT

					m_pImage->SetPixelColor(x, y, newcolor);
				}
			}
		}
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}

void CImageProcessingDoc::OnProcessEqualization()
{
	// TODO: Add a Histogram equalization code here
	if (m_pImage) 
	{

		DWORD width = m_pImage->GetWidth();
		DWORD height = m_pImage->GetHeight();
		RGBQUAD color;	// Save the current color value
		RGBQUAD newcolor;	// After the conversion, save the color value

		//(1) Histogram has already been generated histogram(Omission) 
		//(2) Create a cumulative histogram
		DWORD sum = 0;
		float scale_factor = 255.0 / (width*height);
		DWORD sum_hist[256]; // a cumulative histogram

		for(int i=0; i<256 ; i++)
		{
		// (Coding)

		}


		//(3) Calculating look-up table
		BYTE LUT[256]; // look-up table

		for(int i=0; i<256 ; i++)
		{
		// (Coding)  


		}

		// (4) Image Conversion
		for (DWORD y = 0; y < height; y++) 
		{
			for (DWORD x = 0; x < width; x++) 
			{
				color = m_pImage->GetPixelColor(x, y);
				// (Coding)



				m_pImage->SetPixelColor(x, y, newcolor);
			}
		}		
	}

	CalculateHistogram();
	UpdateAllViews(NULL);
}



void CImageProcessingDoc::OnHistogramSpecification()
{
	// TODO: Add a Histogram specifications code here
	if (m_pImage) 
	{

		DWORD width = m_pImage->GetWidth();
		DWORD height = m_pImage->GetHeight();
		RGBQUAD color;	// Save the current color value
		RGBQUAD newcolor;	// After the conversion, save the color value

		//(1) Histogram has already been generated histogram(Omission) 
		//(2) Create a cumulative histogram
		DWORD sum = 0;
		float scale_factor = 255.0 / (width*height);
		DWORD sum_hist[256]; // a cumulative histogram

		for(int i=0; i<256 ; i++)
		{
		// (coding)

		}


		//(3) Calculating look-up table
		BYTE LUT[256]; // look-up table

		for(int i=0; i<256 ; i++)
		{
		// (coding)  


		}


		// Same as histogram equalization code from here.
		// However, After a cumulative histogram convert to float sum_hist[256] and
		// modifing to be entered in the real number on calculating sum_hist


		//(4) 
		DWORD desired_histogram[256];
		// Making desired_histogram
/*
		//1.
		for (int i=0; i<=255; i++)
		{
			desired_histogram[i]=i;
		}
*/
/*
		//2.
		for (int i=0; i<=255; i++)
		{
			desired_histogram[i]=255-i;
		}
*/
/*
		//3.		
		for (int i=0; i<=127; i++)
		{
			desired_histogram[i]=127-i;
		}
		for (int i=128; i<=255; i++)
		{
			desired_histogram[i]=i-127;
		}
*/

		// redefinition
		sum = 0;
		scale_factor = 255.0 / (width*height);

		//(5) Create the cumulative histogram of histogram you want
		for(int i=0; i<256; i++)
		{
			//(coding)



		}

		//(6) Is the inverse.
		float difference;	// calculate the difference
		int min;	// Calculate the minimum of the difference
		DWORD inv_hist[256];	// reverse-histogram

		//(Hint) will use dual-route.
		// float fabs() Use library functions.
		for(int i=0; i<256; i++)
		{
		//(coding)




		}

		//(7) Create look-up table of reverse-histogram
		for(int i=0; i<256 ; i++)
		{
		//(coding)

		}
		
		

		// (8) Image Conversion
		for (DWORD y = 0; y < height; y++) 
		{
			for (DWORD x = 0; x < width; x++) 
			{
				color = m_pImage->GetPixelColor(x, y);
				// (coding)



				m_pImage->SetPixelColor(x, y, newcolor);
			}
		}		
	} // if(m_pImage)

	CalculateHistogram();
	UpdateAllViews(NULL);

	

}
