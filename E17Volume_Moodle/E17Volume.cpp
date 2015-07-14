#include <iostream>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkFixedPointVolumeRayCastMapper.h>


using namespace std;

int main()
{
    /* Load data file */
    vtkSmartPointer<vtkStructuredPointsReader> dataReader =
            vtkSmartPointer<vtkStructuredPointsReader>::New();
    dataReader->SetFileName("data/Object4.vtk");
    dataReader->Update();


    /* Histogram */
/*
    int size = dataReader->GetOutput()->GetScalarTypeMax();
    int* a = new int[size];
    for (int i = 0; i < size; ++i)
    {
        a[i] = 0;
    }
    for (int x = 0; x < dataReader->GetOutput()->GetDimensions()[0]; ++x)
        for (int y = 0; y < dataReader->GetOutput()->GetDimensions()[1]; ++y)
            for (int z = 0; z < dataReader->GetOutput()->GetDimensions()[2]; ++z)
    {
        int i = dataReader->GetOutput()->GetScalarComponentAsDouble(x, y, z, 0);
        a[i]++;
    }
    for (int i = 0; i < size; ++i)
    {
        std::cout << i << " " << a[i] << std::endl;
    }
    delete[] a;
    exit(0);
*/

    /* Rendering */
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderWindow> renderWin =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWin->SetSize(800, 600);
    renderWin->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWinInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWinInteractor->SetRenderWindow(renderWin);
    renderWinInteractor->Initialize();

    /* Volume Rendering  */
    vtkSmartPointer<vtkColorTransferFunction> colorFunc =
            vtkSmartPointer<vtkColorTransferFunction>::New();

   colorFunc->AddHSVSegment(0,0.0,1.0,1.0,255*0.3333,0.3333,1.0,1.0);
   colorFunc->AddHSVSegment(255*0.3333,0.3333,1.0,1.0,255*0.6666,0.6666,1.0,1.0);
   colorFunc->AddHSVSegment(255*0.6666,0.6666,1.0,1.0,255,1.0,1.0,1.0);

    vtkSmartPointer<vtkPiecewiseFunction> opacityFunc =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
    //opacityFunc->AddSegment(0, 0.0, 255, 1.0);
    opacityFunc->AddSegment(0, 0.0, 19, 0.001);  
    opacityFunc->AddSegment(20, 0.001, 120, 0.01);
    opacityFunc->AddSegment(121, 0.01, 164, .1);
    opacityFunc->AddSegment(165, 0.5, 255, 0.9);

    vtkSmartPointer<vtkPiecewiseFunction> opacityFunc2 =
            vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacityFunc2->AddSegment(0, 0.0, 10, 0.0);
    opacityFunc2->AddSegment(11, 1.0, 255, 1.0);


    vtkSmartPointer<vtkVolumeProperty> property =
            vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetColor(colorFunc);
    property->SetScalarOpacity(opacityFunc);
    property->SetGradientOpacity(opacityFunc2); //cancomment
    property->ShadeOn();

    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> mapper =
        vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
    mapper->SetInputConnection(dataReader->GetOutputPort());
    //mapper->SetImageSampleDistance(1);

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    volume->SetProperty(property);
    volume->SetMapper(mapper);

    renderer->AddVolume(volume);

    renderWinInteractor->Start();

    return 0;
}

