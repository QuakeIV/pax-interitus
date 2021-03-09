using Godot;
using System;
using System.Collections.Generic;
using n = System.Numerics;

//represents an orbital trajectory and corresponding mass
public class OrbitObject : Transform
{
    public static Renderer view;
    
    //TODO: furnish float/double radius pre-calculated
    public ulong orbital_radius     = 0; //in mm, ideally later replaced with more parameters later
    public long  orbital_period     = 0; //in milliseconds
    public long  orbit_clock_offset = 0; //initial position, in milliseconds
    public static Color orbit_color = new Color(0, 1, 0);
    public OrbitObject parent = null; //object which this object orbits
    public Celestial body = null; //object itself (contains mass, null if this is an effectively massless object)
    
    private const int racetrack_points = 64; //64 seems to be a good number of points;
    private long  racetrack_delta_time; //in milliseconds
    private FixedV2D[] rel_racetrack   = new FixedV2D[racetrack_points];
    private List<OrbitObject> children = new List<OrbitObject>();
    
    private static Random rnd = new Random();
    
    public OrbitObject() : this(null, 0) {}
    
    public OrbitObject(Celestial c) : this(null, 0) { body = c; c.parent = this; }
    
    public OrbitObject(OrbitObject p, ulong r, Celestial c) : this(p, r) { body = c; c.parent = this; }
    
    public OrbitObject(OrbitObject p, ulong r)
    {
        parent         = p;
        orbital_radius = r;
        body = null;
        
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
            double period_d = (radius_d*Math.PI*2.0*Math.Sqrt(((radius_d) / (p.body.mass * G))));
            orbital_period = (long)period_d;
            //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
            racetrack_delta_time = orbital_period/racetrack_points;
            //start the planet at a random spot in its orbit
            orbit_clock_offset = (long)(rnd.NextDouble()*period_d);
        }
    }
    
    public void SetCelestial(Celestial c)
    {
        c.parent = this;
        body = c;
    }
    
    public OrbitObject AddChild(OrbitObject c)
    {
        c.parent = this;
        children.Add(c);
        return c;
    }
    
    public OrbitObject AddChild(ulong r)
    {
        OrbitObject c = new OrbitObject(this, r);
        children.Add(c);
        return c;
    }
    
    public OrbitObject AddChild(ulong r, Celestial c)
    {
        OrbitObject child = new OrbitObject(this, r);
        child.body = c;
        c.parent = child;
        children.Add(child);
        return child;
    }
    
    //returns failure
    //this exists to facilitate moving a body to a new parent
    public bool RemoveChild(OrbitObject c)
    {
        if (children.Remove(c))
        {
            //only detaches parent if element was actually a child
            c.parent = null;
            return true;
        }
        else
            return false;
    }
    
    public void DrawOrbit()
    {
        if (orbital_radius * view.currentZoom > 10f) //TODO: bit expensive to constantly convert
        {
            //dont bother drawing the orbit unless its going to be visible
            var draw_points = new Vector2[racetrack_points];
            for (int i = 0; i < racetrack_points; i++)
                draw_points[i] = view.GetScreenCoordinate(rel_racetrack[i] + parent.position);
            
            for (int i = 0; i < racetrack_points - 1; i++)
                view.DrawLine(draw_points[i], draw_points[i + 1], orbit_color, antialiased:true);
            //fencepost
            view.DrawLine(draw_points[racetrack_points-1], draw_points[0], orbit_color, antialiased:true);
        }
    }
    
    //for incrementing orbital calculations
    public long muldiv(long x, long mul, long div)
    {
        //TODO: make this not horribly ineffecient (biginteger is horribly ineffecient)
        var trueval = new n.BigInteger(x);
        trueval*= mul;
        trueval/= div;
        return (long) trueval;
    }
    
    public void DrawPosition()
    {
        //TODO: draw x or something to mark the spot of a barycenter or other virtual orbital location
    }
    
    public void Draw()
    {
        //draw orbit first (bottomost layer)
        if (parent != null)
            DrawOrbit();
        
        //draw children next, so that parent body will supersede them in rendering
        foreach (OrbitObject child in children)
            child.Draw();
        
        //draw own body
        if (body != null)
            body.Draw();
        else
            DrawPosition();
    }
    
    public override void UpdatePosition()
    {
        if (parent != null)
        {
            long orbit_clock = (Universe.time + orbit_clock_offset) % orbital_period;
            int index  = (int)(orbit_clock/racetrack_delta_time);
            int next   = (index + 1) % racetrack_points; //wraps back around to 0 if we are about to overflow
            FixedV2D d = rel_racetrack[next]
                         - rel_racetrack[index];
            
            d.x = muldiv(d.x, orbit_clock%racetrack_delta_time, racetrack_delta_time);
            d.y = muldiv(d.y, orbit_clock%racetrack_delta_time, racetrack_delta_time);
            
            position = d + rel_racetrack[index] + parent.position;
        }
        
        foreach (OrbitObject child in children)
            child.UpdatePosition();
    }
    
    public bool ClickEvent(InputEventMouseButton mouse)
    {
        //try to catch the event ourselves first
        if (body.ClickEvent(mouse))
            return true;
        
        foreach (OrbitObject child in children)
        {
            if (child.ClickEvent(mouse))
                return true;
        }
        return false;
    }
    
    public bool AltClickEvent(InputEventMouseButton mouse)
    {
        GD.Print(mouse);
        return false;
    }
}
