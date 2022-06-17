#include<bits/stdc++.h>
using namespace std;

class Shape {
    public:
       virtual void Draw() = 0;
       static Shape* shapeFactory(string type);
};

class Circle : public Shape {
    public:
        void Draw() {
           cout << "I am circle " <<endl;
        }
        friend class Shape;
};

class Square : public Shape {
    public :
        void Draw() {
           cout << "I am square " <<endl;
        }
        friend class Shape;
}

Shape* Shape::shapeFactory(string type) {
    if( type == "circle") {
        return new Circle();
    }
    if ( type == "square"){
        return new Square();
    }
    return NULL;
}

int main() {
    Shape* obj1 = Shape::shapeFactory("circle");
    Shape* obj2 = Shape::shapeFactory("square");
    obj1->Draw();
    obj2->Draw();
    return 0;
}
