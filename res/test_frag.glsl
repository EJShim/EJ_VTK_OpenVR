//VTK::System::Dec
//VTK::Output::Dec

varying vec3 normalVCVSOutput;
varying vec3 vPos;

uniform vec3 diffuseColorUniform;


void main()
{
  gl_FragColor = vec4(normalVCVSOutput, 1.0);
}
