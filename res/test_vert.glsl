//VTK::System::Dec
attribute vec4 vertexMC;

//VTK::Normal::Dec
uniform mat4 MCDCMatrix;
uniform mat4 VCDCMatrix;

varying vec4 vPos;

void main()
{
  //Varying normalMC
  normalVCVSOutput = normalMC;

  vPos = MCDCMatrix * vertexMC;
  gl_Position = vPos;
}
