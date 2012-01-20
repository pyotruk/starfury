#ifndef DETECTOR_HF_H
#define DETECTOR_HF_H
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include "utils/cvwrap.h"
#include "math/astrocalc.h"
#include "boxes/frame.h"
#include "boxes/artifact.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace detector_hf
{
void findTargets(Frame &f,                 //input
                 ArtifactVector &av,       //output
                 const int minSquare = 9,
                 const bool MTI = false,   /* Moving-Target Indication (�������� ���������� �����)
                                              ������ ��� ������������ ����� */
                 const int oblongDiff = 2); /* ������������� ��������� ������
                                               �� ���������� ����� */
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DETECTOR_HF_H
