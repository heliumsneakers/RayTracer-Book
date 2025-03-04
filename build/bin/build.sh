cd ..
make
cd bin/
rm image.ppm
./RayTracer > image.ppm
open image.ppm

