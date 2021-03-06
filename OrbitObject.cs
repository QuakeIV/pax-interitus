using Godot;
using System;
using System.Collections.Generic;
using n = System.Numerics;

//represents an orbital trajectory and corresponding mass
public class OrbitObject
{
    public FixedV2D position = new FixedV2D();
    
    public static Renderer view;
    
    public ulong orbital_radius     = 0; //in mm, ideally later replaced with more parameters later
    public ulong mass               = 0; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    public long  orbital_period     = 0; //in milliseconds
    public long  orbit_clock        = 0; //in milliseconds
    public static Color orbit_color = new Color(0, 1, 0);
    public OrbitObject parent = null; //object which this object orbits
    public Celestial body; //object itself (null if this is a barycenter for instance)
    
    private const int racetrack_points = 64; //64 seems to be a good number of points;
    private long  racetrack_delta_time; //in milliseconds
    private FixedV2D[] rel_racetrack   = new FixedV2D[racetrack_points];
    private List<OrbitObject> children = new List<OrbitObject>();
    
    private static Random rnd = new Random();
    
    public OrbitObject(ulong m) : this(null, 0, m) {}
    
    public OrbitObject(ulong m, Celestial c) : this(null, 0, m) { body = c; c.parent = this; }
    
    public OrbitObject(OrbitObject p, ulong r, ulong m, Celestial c) : this(p, r, m) { body = c; c.parent = this; }
    
    public OrbitObject(OrbitObject p, ulong r, ulong m)
    {
        parent         = p;
        orbital_radius = r;
        mass           = m;
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
            double period_d = (radius_d*Math.PI*2.0*Math.Sqrt(((radius_d) / (p.mass * G))));
            orbital_period = (long)period_d;
            //worst case roundoff error here is I think racetrack_points - 1 milliseconds (this is the true reference for orbital period in terms of calculating position)
            racetrack_delta_time = orbital_period/racetrack_points;
            //start the planet at a random spot in its orbit
            orbit_clock = (long)(rnd.NextDouble()*period_d);
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
    
    public OrbitObject AddChild(ulong r, ulong m)
    {
        OrbitObject c = new OrbitObject(this, r, m);
        children.Add(c);
        return c;
    }
    
    public OrbitObject AddChild(ulong r, ulong m, Celestial c)
    {
        OrbitObject child = new OrbitObject(this, r, m);
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
                view.DrawLine(draw_points[i], draw_points[i + 1], orbit_color);
            //fencepost
            view.DrawLine(draw_points[racetrack_points-1], draw_points[0], orbit_color);
        }
    }
    
    //for incrementing orbital calculations
    public long muldiv(long x, long mul, long div)
    {
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
        //draw children first, so that parents will supersede them in rendering
        foreach (OrbitObject child in children)
            child.Draw();
        
        if (parent != null)
            DrawOrbit();
        
        if (body != null)
            body.Draw();
        else
            DrawPosition();
    }
    
    public void UpdatePosition(long deltaTime)
    {
        if (parent != null)
        {
            int index  = (int)(orbit_clock/racetrack_delta_time);
            int next   = (index + 1) % racetrack_points; //wraps back around to 0 if we are about to overflow
            FixedV2D d = rel_racetrack[next] - rel_racetrack[index];
            
            d.x = muldiv(d.x, orbit_clock%racetrack_delta_time, racetrack_delta_time);
            d.y = muldiv(d.y, orbit_clock%racetrack_delta_time, racetrack_delta_time);
            
            position = d + rel_racetrack[index] + parent.position;
            
            //just increment by a const for now, pending a better solution
            orbit_clock += deltaTime;
            orbit_clock %= orbital_period;
        }
        
        foreach (OrbitObject child in children)
            child.UpdatePosition(1000);
    }
}
