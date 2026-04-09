#include "modellistclass.h"


ModelListClass::ModelListClass()
{
    m_ModelInfoList = 0;
}


ModelListClass::ModelListClass(const ModelListClass& other)
{
}


ModelListClass::~ModelListClass()
{
}

void ModelListClass::Initialize(int numModels)
{   
    m_modelCount = numModels * numModels;
    m_ModelInfoList = new ModelInfoType[m_modelCount];

    srand((unsigned int)time(NULL));

    for (int i = 0, x = 0, z = 0; i < m_modelCount; i++, x++)
    {
        if (x == numModels) {
            x = 0;
            z++;
        }

        m_ModelInfoList[i].positionX = x * 3.5;
        m_ModelInfoList[i].positionY = i * 0.03;
        m_ModelInfoList[i].positionZ = z * 3.5;
    }

}

void ModelListClass::Shutdown()
{
    if (m_ModelInfoList)
    {
        delete[] m_ModelInfoList;
        m_ModelInfoList = 0;
    }
}

int ModelListClass::GetModelCount()
{
    return m_modelCount;
}

void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ)
{
    positionX = m_ModelInfoList[index].positionX;
    positionY = m_ModelInfoList[index].positionY;
    positionZ = m_ModelInfoList[index].positionZ;
    return;
}
