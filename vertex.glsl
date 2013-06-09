varying vec3 p;

void main()
{
    gl_Position = gl_ProjectionMatrix * gl_Vertex;
    p = gl_Vertex.xyz;
}
