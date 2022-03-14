// Vector 2
Vec2 = { x : 0, y :0 };

Vec2.New = function(x = 0, y = 0) 
{
	let res = Object.create(Vec2);
	res.x = x; res.y = y;
	return res;
}

Vec2.Add = function(other)
{
	let res = Vec2.New(this.x, this.y);
	res.x += other.x;
	res.y += other.y;
	return res;
}

Vec2.Sub = function(other)
{
	let res = Vec2.New(this.x, this.y);
	res.x -= other.x;
	res.y -= other.y;
	return res;
}

Vec2.Mul = function(other)
{
	let res = Vec2.New(this.x, this.y);
	res.x *= other.x;
	res.y *= other.y;
	return res;
}

Vec2.Div = function(other)
{
	let res = Vec2.New(this.x, this.y);
	res.x /= other.x;
	res.y /= other.y;
	return res;
}

Vec2.Dot = function(other)
{
	return this.x*other.x + this.y*other.y;
}

Vec2.Cross = function(other)
{
	return this.x*other.x - this.y*other.y;
}

Vec2.SqrLength = function(other)
{
	return this.x*this.x + this.y*this.y;
}

Vec2.Length = function(other)
{
	return Math.sqrt(this.x*this.x + this.y*this.y);
}

// Rectangle
Rect = { left : 0, bottom : 0, right : 0, top : 0 }

Rect.New = function(left = 0, bottom = 0, right = 0, top = 0) 
{
	let res = Object.create(Rect);
	res.left = left; 
	res.bottom = bottom;
	res.right = right;
	res.top = top;
	return res;
}

Rect.Width = function()
{
	return Math.abs(this.right - this.left);
}

Rect.Height = function()
{
	return Math.abs(this.bottom - this.top);
}

Rect.Size = function()
{
	return Vec2.New(this.Width(), this.Height());
}

Rect.Center = function()
{
	return Vec2.New((this.left + this.right)/2, (this.bottom + this.top)/2);
}

Rect.IsInside = function(vec)
{
	return vec.x > this.left && vec.x < this.right && vec.y > this.bottom && vec.y < this.top;
}

Rect.IsIntersects = function(other)
{
	return !(this.right < other.left || this.left > other.right || this.bottom > other.top || this.top < other.bottom);
}

Rect.LeftTop = function()
{
	return Vec2.New(this.left, this.top);
}

Rect.LeftBottom = function()
{
	return Vec2.New(this.left, this.bottom);
}

Rect.RightTop = function()
{
	return Vec2.New(this.right, this.top);
}

Rect.RightBottom = function()
{
	return Vec2.New(this.right, this.bottom);
}

// Border
Border = { left : 0, bottom : 0, right : 0, top : 0 }

Border.New = function(left = 0, bottom = 0, right = 0, top = 0) 
{
	let res = Object.create(Border);
	res.left = left; 
	res.bottom = bottom;
	res.right = right;
	res.top = top;
	return res;
}

Border.LeftTop = function()
{
	return Vec2.New(this.left, this.top);
}

Border.LeftBottom = function()
{
	return Vec2.New(this.left, this.bottom);
}

Border.RightTop = function()
{
	return Vec2.New(this.right, this.top);
}

Border.RightBottom = function()
{
	return Vec2.New(this.right, this.bottom);
}

// Color4
Color4 = { r : 255, g : 255, b : 255, a : 255 }

Color4.New = function(r = 255, g = 255, b = 255, a = 255) 
{
	let res = Object.create(Color4);
	res.r = r; 
	res.g = g;
	res.b = b;
	res.a = a;
	return res;
}

Color4.Red = function() { return Color4.New(255, 0, 0, 255); }
Color4.Green = function() { return Color4.New(0, 255, 0, 255); }
Color4.Blue = function() { return Color4.New(0, 0, 255, 255); }
Color4.White = function() { return Color4.New(255, 255, 255, 255); }
Color4.Black = function() { return Color4.New(0, 0, 0, 255); }