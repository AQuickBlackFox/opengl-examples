## Notes

### Introduction
This page contains notes to my observations on opengl experiments.

### gl_VertexID
I was trying to use `gl_VertexID` as `threadIdx.x` (from CUDA) for vertices.
When I tried 

![alt-text][gl_VertexID-1] 

the output is off interms of both shading and vertex positions

Then I added more values to `vertices` to make it a perfect point.

![alt-text][gl_VertexID-2] 

It started to make sense that `gl_VertexID` always access a buffer with a stride of 4x 32-bit elements.

To confirm my observations, I changed color code to match vertices behavior

![alt-text][gl_VertexID-3]

[gl_VertexID-1]: https://github.com/gpu0/opengl-examples/blob/master/docs/gl_VertexID-1.jpg
[gl_VertexID-2]: https://github.com/gpu0/opengl-examples/blob/master/docs/gl_VertexID-2.jpg
[gl_VertexID-3]: https://github.com/gpu0/opengl-examples/blob/master/docs/gl_VertexID-3.jpg
