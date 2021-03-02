using Godot;
using System;
using System.Collections.Generic;

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
    public ulong mass = 0; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    public ulong orbital_period = 0; //in milliseconds
    public OrbitObject parent; //object which this object orbits
    
    private FixedV2D rel_position  = new FixedV2D();
    private List<FixedV2D> rel_racetrack = new List<FixedV2D>();
    private List<OrbitObject> children = new List<OrbitObject>();
    
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
            const int racetrack_points = 32; //32 seems to be a good number of points;
            for (int i = 0; i < racetrack_points; i++)
            {
                
            }
            
            //period in milliseconds (doing calculation in double land, and then converting back to the integral reference frame)
            const double G = 66743000; //gravitational constant, in cubic mm per exagram millisecond squared
            double temp_radius = (double)orbital_radius;
            double temp_mass   = (double)p.mass;
            //did some algebra, assuming newtonian gravity, newtonian motion this should yield orbital period
            double period      = temp_radius*Math.PI*2.0*Math.Sqrt(((temp_radius) / (temp_mass * G)));
            orbital_period     = (ulong)period;
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
    
    public void UpdatePosition()
    {
        
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

        //this can be replaced with the orbit racetrack whenever that gets calculated
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
    
    
    private float currentZoom = 0.00000000000001f; //1.0f;
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
