class Vec2
{
	constructor(x = 0, y = 0)
	{
		this.x = x;
		this.y = y;
	}

	Add(other)
	{
		let res = new Vec2(this.x, this.y);
		res.x += other.x;
		res.y += other.y;
		return res;
	}

	Sub(other)
	{
		let res = new Vec2(this.x, this.y);
		res.x -= other.x;
		res.y -= other.y;
		return res;
	}

	Mul(other)
	{
		let res = new Vec2(this.x, this.y);
		res.x *= other.x;
		res.y *= other.y;
		return res;
	}

	Div(other)
	{
		let res = new Vec2(this.x, this.y);
		res.x /= other.x;
		res.y /= other.y;
		return res;
	}

	Dot(other)
	{
		return this.x*other.x + this.y*other.y;
	}

	Cross(other)
	{
		return this.x*other.x - this.y*other.y;
	}

	SqrLength()
	{
		return this.x*this.x + this.y*this.y;
	}

	Length()
	{
		return Math.sqrt(this.x*this.x + this.y*this.y);
	}
}

class Rect
{
	constructor(left = 0, bottom = 0, right = 0, top = 0) 
	{
		this.left = left; 
		this.bottom = bottom;
		this.right = right;
		this.top = top;
	}

	Width()
	{
		return Math.abs(this.right - this.left);
	}

	Height()
	{
		return Math.abs(this.bottom - this.top);
	}

	Size()
	{
		return new Vec2(this.Width(), this.Height());
	}

	Center()
	{
		return new Vec2((this.left + this.right)/2, (this.bottom + this.top)/2);
	}

	IsInside(vec)
	{
		return vec.x > this.left && vec.x < this.right && vec.y > this.bottom && vec.y < this.top;
	}

	IsIntersects(other)
	{
		return !(this.right < other.left || this.left > other.right || this.bottom > other.top || this.top < other.bottom);
	}

	LeftTop()
	{
		return new Vec2(this.left, this.top);
	}

	LeftBottom()
	{
		return new Vec2(this.left, this.bottom);
	}

	RightTop()
	{
		return new Vec2(this.right, this.top);
	}

	RightBottom()
	{
		return new Vec2(this.right, this.bottom);
	}
}

class Border
{
	constructor(left = 0, bottom = 0, right = 0, top = 0) 
	{
		this.left = left; 
		this.bottom = bottom;
		this.right = right;
		this.top = top;
	}

	LeftTop()
	{
		return new Vec2(this.left, this.top);
	}

	LeftBottom()
	{
		return new Vec2(this.left, this.bottom);
	}

	RightTop()
	{
		return new Vec2(this.right, this.top);
	}

	RightBottom()
	{
		return new Vec2(this.right, this.bottom);
	}
}

class Color4
{
	constructor(r = 255, g = 255, b = 255, a = 255) 
	{
		this.r = r; 
		this.g = g;
		this.b = b;
		this.a = a;
	}

	static Red() { return new Color4(255, 0, 0, 255); }
	static Green() { return new Color4(0, 255, 0, 255); }
	static Blue() { return new Color4(0, 0, 255, 255); }
	static White() { return new Color4(255, 255, 255, 255); }
	static Black() { return new Color4(0, 0, 0, 255); }
}

Math.randomBetween = function(a, b)
{
	return a + Math.random()*(b - a);
}