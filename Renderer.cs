using Godot;
using System;
using System.Collections.Generic;
using n = System.Numerics;

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


public class OrbitObject
{
    public FixedV2D position = new FixedV2D();
    
    public ulong orbital_radius = 0; //in mm, ideally later replaced with more parameters later
    public ulong mass           = 0; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    public long  orbital_period = 0; //in milliseconds
    public long  orbit_clock    = 0; //in milliseconds
    public OrbitObject parent; //object which this object orbits
    
    private const int racetrack_points = 64; //64 seems to be a good number of points;
    private long  racetrack_delta_time = 64; //in milliseconds
    private FixedV2D[] rel_racetrack   = new FixedV2D[racetrack_points];
    private List<OrbitObject> children = new List<OrbitObject>();
    
    private static Random rnd = new Random();
    
    public OrbitObject(ulong mass) : this(null, 0, mass) {}
    
    public OrbitObject(OrbitObject p, ulong r, ulong m)
    {
        parent         = p;
        orbital_radius = r;
        mass           = m;
        
        // only define an orbit if there is a parent object
        if (parent != null)
        {
            //doing sine/cosine math in double land, and then converting back to the fixed reference frame
            double radius_d = (double) orbital_radius;
            for (int i = 0; i < racetrack_points; i++)
            {
                double angle = (2.0*Math.PI*i)/racetrack_points;
                rel_racetrack[i] = new FixedV2D(radius_d*Math.Cos(angle), radius_d*Math.Sin(angle));
            }
            
            //period in milliseconds (doing calculation in double land, and then converting back to the integral reference frame)
            const double G = 66743000; //gravitational constant, in cubic mm per exagram millisecond squared
            //did some algebra, assuming newtonian gravity, newtonian motion this should yield orbital period
            double period_d = (radius_d*Math.PI*2.0*Math.Sqrt(((radius_d) / (p.mass * G))));
            orbital_period = (long)period_d;
            //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
            racetrack_delta_time = orbital_period/racetrack_points;
            //start the planet at a random spot in its orbit
            orbit_clock = (long)(rnd.NextDouble()*period_d);
        }
    }
    
    public OrbitObject AddChild(OrbitObject c)
    {
        c.parent = this;
        children.Add(c);
        return c;
    }
    
    public OrbitObject AddChild(ulong r, ulong m)
    {
        OrbitObject c = new OrbitObject(this, r, m);
        children.Add(c);
        return c;
    }
    
    //returns failure
    //doesn't remove parent if element was not a child
    //this exists to facilitate moving a body to a new parent
    public bool RemoveChild(OrbitObject c)
    {
        if (children.Remove(c))
        {
            c.parent = null;
            return true;
        }
        else
            return false;
    }
    
    //for incrementing orbital calculations
    public long muldiv(long x, long mul, long div)
    {
        var trueval = new n.BigInteger(x);
        trueval*= mul;
//        if ((long.MaxValue / x) < mul || )
//        {
            //we will overflow, handle
            
            long hix = x >> 32;
            long lox = x & (long)(uint.MaxValue | (long)0b1000000000000000000000000000000000000000000000000000000000000000);
            long him = mul >> 32;
            long lom = mul & uint.MaxValue;
            
            long hi  = hix * him;
            long mi1 = hix * lom;
            long mi2 = him * lox;
            long lo  = lox * lom;
            
            //add higher 32 bits of middle into lower 32 bits of high
            hi += mi1 >> 32;
            hi += mi2 >> 32;
            mi1 &= uint.MaxValue;
            mi2 &= uint.MaxValue;
            mi1 += mi2;
            mi1 += lo >> 32; //add upper order bits of lo into low end of mid to catch overflow
            hi += mi1 >> 32; //add mid overflows back into hi
            
            //low order middle is now equal to high order lo, so overwrite
            lo = (lo & uint.MaxValue) | (mi1 << 32);
            
            var calc = new n.BigInteger(hi);
            calc <<= 64;
            calc += lo;
            
            if (calc != trueval)
            {
                GD.Print("MATH ERROR IN MULDIV");
                GD.Print(calc);
                GD.Print(trueval);
            }
            else
            {
                GD.Print("WIN");
            }
            
            return 0;
            
//        }
//        else
//        {
//            //we will not overflow, proceed as normal
//            return (x * mul) / div;
//        }
    }
    
    public void UpdatePosition()
    {
        int index  = (int)(orbit_clock/racetrack_delta_time);
        int next   = (index + 1) % racetrack_points; //wraps back around to 0 if we are about to overflow
        FixedV2D d = rel_racetrack[index] - rel_racetrack[next];
        
        muldiv(d.x, orbit_clock%racetrack_delta_time, racetrack_delta_time);
        
        //just increment by a const for now, pending a better solution
        orbit_clock += 1000000;
        orbit_clock %= orbital_period;
    }
}

public class Renderer : Godot.Node2D
{
    private FixedV2D camera_position = new FixedV2D();
    
    private FixedV2D test_star = new FixedV2D(0, 0);
    // Declare member variables here. Examples:
    // private int a = 2;
    // private string b = "text";

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        OrbitObject thing = new OrbitObject(1000000);
        thing.muldiv(long.MaxValue, 100, 1);
        thing.muldiv(long.MaxValue, 99, 1);
        thing.muldiv(long.MaxValue, 11, 1);
        thing.muldiv((long)Math.Pow(2,32), (long)Math.Pow(2,33), 1);
        thing.muldiv(long.MaxValue, 100, 1);
        thing.muldiv(long.MaxValue, 1337, 1);
        thing.muldiv(long.MaxValue, 1087349213, 1);
        thing.muldiv(long.MaxValue, 100000, 1);
        thing.muldiv(12908015, 980923491,1);
        thing.muldiv(19374, 109840198234,1);
        thing.muldiv(12938012, 98713981309,1);
        thing.muldiv(20187349821, 98713981309,1);
        GD.Print("DONE");
    }

    Vector2 GetScreenCoordinate(FixedV2D a)
    {
        FixedV2D v = a - camera_position;
        Vector2 screen = GetViewportRect().Size;
        screen.x = screen.x / 2.0f;
        screen.y = screen.y / 2.0f;
        
        Vector2 zoom_pos = new Vector2(v.x * currentZoom, v.y * currentZoom);
        return zoom_pos + screen;
    }

    // Called every frame. 'delta' is the elapsed time since the previous frame.
    public override void _Process(float delta)
    {
        //dont need to update every frame, just every time something changes
//        Update();
    }

//    public void DrawCircleArc(Vector2 center, float radius, float angleFrom, float angleTo, Color color)
//    {
//        int nbPoints = 32;
//        var pointsArc = new Vector2[nbPoints];
//
//        for (int i = 0; i < nbPoints; ++i)
//        {
//            float anglePoint = Mathf.Deg2Rad(angleFrom + i * (angleTo - angleFrom) / nbPoints - 90f);
//            pointsArc[i] = center + new Vector2(Mathf.Cos(anglePoint), Mathf.Sin(anglePoint)) * radius;
//        }
//
//        for (int i = 0; i < nbPoints - 1; ++i)
//            DrawLine(pointsArc[i], pointsArc[i + 1], color);
//    }

    public void DrawOrbit(OrbitObject orbit, Color color)
    {
        FixedV2D center = new FixedV2D(); //should be parent position (not implemented yet), translate coordinates appropriately
        const int nbPoints = 32;
        var points = new FixedV2D[nbPoints];

        for (int i = 0; i < nbPoints; ++i)
        {
            double anglePoint = (2.0*Math.PI * i) / nbPoints;
            double radius = (double)orbit.orbital_radius;
            points[i] = center + new FixedV2D((long)(Math.Cos(anglePoint) * radius), (long)(Math.Sin(anglePoint) * radius));
        }
        
        var draw_points = new Vector2[nbPoints];
        for (int i = 0; i < nbPoints; ++i)
            draw_points[i] = GetScreenCoordinate(points[i]);
        
        for (int i = 0; i < nbPoints - 1; ++i)
            DrawLine(draw_points[i], draw_points[i + 1], color);
        //fencepost
        DrawLine(draw_points[nbPoints-1], draw_points[0], color);
    }

    public override void _Draw()
    {
        OrbitObject sol = new OrbitObject(1988500000000000);
        OrbitObject earth = new OrbitObject(sol, 149598023000000, 5972200000);
        Color orbit_color = new Color(0, 1, 0);
        DrawOrbit(earth, orbit_color);
        
        Vector2 center = GetScreenCoordinate(test_star);
        float radius = 695700000000f*currentZoom;
        //enforce minimum dot radius
        if (radius < 5.0f)
            radius = 5.0f;
        
        Color sun_color = new Color(226.0f/255.0f, 223.0f/255.0f, 24.0f/255.0f);
        DrawCircle(center, radius, sun_color);
    }
    
    
    private float currentZoom = 0.000000000001f; //1.0f;
    // member variables here
    private Vector2 mouse1;
    private bool pressed = false;
    
    public override void _UnhandledInput(InputEvent @event)
    {
//        GD.Print(@event);
        // Mouse in viewport coordinates.
        if (@event is InputEventMouseButton eventMouseButton)
        {
            if (eventMouseButton.Pressed && (ButtonList)eventMouseButton.ButtonIndex == ButtonList.Left)
            {
                mouse1 = eventMouseButton.Position;
                pressed = true;
            }
            else
                pressed = false;
        }
        else if (@event is InputEventMouseMotion eventMouseMotion)
        {
            if (pressed)
            {
                Vector2 delta = eventMouseMotion.Position - mouse1;
                delta.x /= currentZoom;
                delta.y /= currentZoom;
                camera_position -= delta;
                mouse1 = eventMouseMotion.Position;
                Update();
            }
        }
        if (Input.IsActionPressed("zoom_out"))
        {
            currentZoom*=0.8f;
            GD.Print("zoom factor of " + ((1f/currentZoom)/1000000f) + "km per pixel");
            Update();
        }
        
        if (Input.IsActionPressed("zoom_in"))
        {
            currentZoom/=0.8f;
            GD.Print("zoom factor of " + ((1f/currentZoom)/1000000f) + "km per pixel");
            Update();
        }

        // Print the size of the viewport.
//        GD.Print("Viewport Resolution is: ", GetViewportRect().Size);
    }
    
    private void _on_Viewport_size_changed()
    {
        Update();
    }
}
