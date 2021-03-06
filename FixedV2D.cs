using Godot;
using System;

// distance units for the purposes of the solar system sim are in millimeters
public class FixedV2D
{
    public long x; //in mm
    public long y; //in mm
    
    public FixedV2D(long nx, long ny)
    {
        x = nx;
        y = ny;
    }
    
    public FixedV2D(double nx, double ny)
    {
        x = (long)nx;
        y = (long)ny;
    }
    
    public FixedV2D(FixedV2D other)
    {
        x = other.x;
        y = other.y;
    }
    
    public FixedV2D(Vector2 other)
    {
        x = (long)other.x;
        y = (long)other.y;
    }
    
    public FixedV2D()
    {
        x = 0;
        y = 0;
    }
    
    public static FixedV2D operator +(FixedV2D a) => new FixedV2D(a.x, a.y);
    public static FixedV2D operator -(FixedV2D a) => new FixedV2D(-a.x, -a.y);
    
    public static FixedV2D operator +(FixedV2D a, FixedV2D b) => new FixedV2D(a.x + b.x, a.y + b.y);
    public static FixedV2D operator -(FixedV2D a, FixedV2D b) => new FixedV2D(a.x - b.x, a.y - b.y);
    public static FixedV2D operator +(FixedV2D a, Vector2 b) => new FixedV2D(a.x + (long)Math.Round(b.x), a.y + (long)Math.Round(b.y));
    public static FixedV2D operator -(FixedV2D a, Vector2 b) => new FixedV2D(a.x - (long)Math.Round(b.x), a.y - (long)Math.Round(b.y));
    public static FixedV2D operator *(FixedV2D a, FixedV2D b) => new FixedV2D(a.x * b.x, a.y * b.y);
    public static FixedV2D operator /(FixedV2D a, FixedV2D b) => new FixedV2D(a.x / b.x, a.y / b.y);
}
