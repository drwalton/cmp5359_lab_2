#include <iostream>
#include <lodepng.h>
#include <fstream>
#include <sstream>

// The goal for this lab is to draw a triangle mesh loaded from an OBJ file from scratch,
// building on the image drawing code from last week's lab.
// The mesh consists of a list of 3D vertices, that describe the points forming the mesh.
// It also has a list of triangle indices, that determine which vertices are used to form each triangle.
// For now, we will just try to draw all the vertices as points, ignoring the indices.

// *** Your Code Here! ***
// * Task 1: Write your own class to store 3D floating-point vectors!
//           Your class will be used to store the vertex positions loaded from the OBJ file.
class Vector
{
public:
	Vector()
		:x_(0), y_(0), z_(0)
	{
		// YOUR CODE HERE
	}
	Vector(float x, float y, float z)
		:x_(x), y_(y), z_(z)
	{
		// YOUR CODE HERE
	}

	// Implement this method to add two vectors.
	Vector operator+(const Vector& other) const
	{
		// YOUR CODE HERE
		return Vector(x_ * other[0], y_ * other[1], z_ * other[2]);
	}

	// Multiply the vector by a scalar.
	Vector operator*(float scalar) const
	{
		// YOUR CODE HERE
		return Vector(x_ * scalar, y_ * scalar, z_ * scalar);
	}

	// Get a component of the vector
	float& operator[](int i)
	{
		if (i == 0) return x_;
		if (i == 1) return y_;
		if (i == 2) return z_;
		else throw std::runtime_error("Oh no vector issues");
		// YOUR CODE HERE
	}

	float operator[](int i) const
	{
		if (i == 0) return x_;
		if (i == 1) return y_;
		if (i == 2) return z_;
		else throw std::runtime_error("Oh no vector issues");
		// YOUR CODE HERE
	}


private:
	// Add the data to store the x, y and z coordinates of the vector.
	float x_, y_, z_;
};

// Note: adding this version of operator* will let us do scalar * vector, as well as vector * scalar.
// see how it makes use of the operator* you define above.
Vector operator*(float scalar, const Vector& vector)
{
	return vector * scalar;
}

// This tells us how to add the vector to a stream
// For example, how to print the vector with std::cout.
std::ostream& operator<<(std::ostream& stream, const Vector& v)
{
	stream << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
	return stream;
}

void setPixel(std::vector<uint8_t>& image, int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
{
	int pixelIdx = x + y * width;
	image[pixelIdx * 4 + 0] = r;
	image[pixelIdx * 4 + 1] = g;
	image[pixelIdx * 4 + 2] = b;
	image[pixelIdx * 4 + 3] = a;
}

int main()
{
	std::string outputFilename = "output.png";

	const int width = 512, height = 512;
	const int nChannels = 4;

	// Setting up an image buffer
	// This std::vector has one 8-bit value for each pixel in each row and column of the image, and
	// for each of the 4 channels (red, green, blue and alpha).
	// Remember 8-bit unsigned values can range from 0 to 255.
	std::vector<uint8_t> imageBuffer(height*width*nChannels);

	// This line sets the memory block occupied by the image to all zeros.
	memset(&imageBuffer[0], 0, width * height * nChannels * sizeof(uint8_t));

	std::string bunnyFilename = "../models/stanford_bunny_simplified.obj";

	std::ifstream bunnyFile(bunnyFilename);


	// *** Task 2 ***
	// Your next task is to load all the vertices from the OBJ file.
	// I've given you some starter code here that reads through each line of the
	// OBJ file and makes it into a stringstream.
	// For these V lines, you should load the X, Y and Z coordinates into a new vector
	// and push it back into your array of vertices.
	std::vector<Vector> vertices;
	std::string line;
	while (!bunnyFile.eof())
	{
		std::getline(bunnyFile, line);
		std::stringstream lineSS(line.c_str());
		char lineStart;
		lineSS >> lineStart;
		if (lineStart == 'v') {
			Vector v;
			for (int i = 0; i < 3; ++i) lineSS >> v[i];
			vertices.push_back(v);

			std::cout << v << std::endl;
		}
	}

	for (auto& v : vertices) {
		// **** Task 3 ****
		// Finally, let's draw the vertices!
		// For each vertex, we need to call setPixel to draw a point to the screen.
		// You should end up with an output similar to example.png.
		// Hint 1: You just need to use the x and y components of the vectors, you can ignore the z component for now.
		// Hint 2: Note the mesh coordinates range from about -0.5 to 0.5 in each axis (x, y and z). However,
		//         your image coordinates go from 0 to 512. You probably want to add on half of the width and height to move
		//         your vertices towards the centre of the screen, and multiply by a value (about 200 or so) to make the mesh
		//         big enough to see.

		int x = v[0] * 250 + width / 2;
		int y = -v[1] * 250 + height / 2;
		setPixel(imageBuffer, x, y, width, height, 255, 255, 255);
	}


	// *** Encoding image data ***
	// PNG files are compressed to save storage space. 
	// The lodepng::encode function applies this compression to the image buffer and saves the result 
	// to the filename given.
	int errorCode;
	errorCode = lodepng::encode(outputFilename, imageBuffer, width, height);
	if (errorCode) { // check the error code, in case an error occurred.
		std::cout << "lodepng error encoding image: " << lodepng_error_text(errorCode) << std::endl;
		return errorCode;
	}

	return 0;
}
