/*

 Input: Circles or Rectangles with coordinates
 Output: an svg file with the drawing of circles or rectangles

*/
#include <iostream>
#include <vector>
using namespace std; 

class Shape
{
    public:
    virtual ~Shape(void) {} 
    virtual void draw(void) const = 0; 
};

class Rectangle : public Shape 
{
    private:
        int lowLeftX; 
        int lowLeftY; 
        int width; 
        int height; 
    public:
        Rectangle(int x, int y, int w, int h): lowLeftX(x), lowLeftY(y), width(w), height(h) {}
        void draw(void) const { cout << "<rect x=\"" << lowLeftX << "\" y=\"" << lowLeftY << "\" width=\"" << width << "\" height=\"" << height << "\"/>" << endl;  }
        ~Rectangle(void) {}
};

class Circle : public Shape
{
    private:
        int cenX;
        int cenY; 
        int radius; 
    public:
        Circle(int x, int y, int r): cenX(x), cenY(y), radius(r) {}
        void draw(void) const { cout << "<circle cx=\"" << cenX << "\" cy=\"" << cenY <<"\" r=\"" << radius << "\"/>" << endl; }
        ~Circle(void) {}
};

int main() {
    string svgStart = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n<svg width=\"700\" height=\"600\"\nxmlns=\"http://www.w3.org/2000/svg\">\n<g transform=\"matrix(1,0,0,-1,50,550)\"\nfill=\"white\" fill-opacity=\"0.5\" stroke=\"black\" stroke-width=\"2\">\n<rect fill=\"lightgrey\" x=\"0\" y=\"0\" width=\"600\" height=\"500\"/>\n";
    string svgEnd = "</g>\n</svg>\n";
    vector <Shape*> v; 

    char shape; 
    cin >> shape; 
    
    while (cin)
    {
        if (shape == 'R')
        {   
            int x; 
            int y; 
            int w; 
            int h; 

            cin >> x >> y >> w >> h; 
            v.push_back(new Rectangle(x, y, w, h)); 
        } 
        else if (shape == 'C')
        {
            int x;
            int y; 
            int r; 

            cin >> x >> y >> r; 
            v.push_back(new Circle(x,y,r));
        }
        cin >> shape; 
    }

    vector <Shape*>::iterator iter = v.begin();
    cout << svgStart; 
    while(iter != v.end())
    {
        (*iter)->draw(); 
        *iter++; 
    }

    for (int index = 0; index < v.size(); index++)
    {
        delete (v[index]); 
    }
    v.clear();


    cout << svgEnd; 
}
