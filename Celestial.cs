using Godot;
using System;

// planet, star, or other celestial objects (stars and planets probably to be split later)
public class Celestial
{
    public static Renderer view;
    public OrbitObject parent;
    
    public ulong mass = 1; //in exagrams (10^18 grams) (quadrillions (10^15) of kgs), ideally later replaced with more parameters later
    public ulong radius; //in mm, radius of the object itself
    //TODO: furnish pre-calculated double/float radius
    public Color color;
    
    // 
    private DateTime last_click;
    
    public Celestial(ulong r, ulong m, Color c)
    {
        radius = r;
        mass = m;
        color = c;
    }
    
    public Celestial(ulong rad, ulong m, byte r, byte g, byte b)
    {
        radius = rad;
        mass = m;
        color = new Color(r/255.0f, g/255.0f, b/255.0f);
    }
    
    public Celestial(ulong r, ulong m)
    {
        radius = r;
        mass = m;
        color = new Color(9.0f/255.0f, 76.0f/255.0f, 13.0f/255.0f);
    }
    
    public float display_radius()
    {
        float rad = radius*view.currentZoom;
        
        //enforce minimum dot radius
        if (rad < 5.0f)
            rad = 5.0f;
        
        return rad;
    }
    
    public void Draw()
    {
        //nothing to draw if following is true (only bother drawing if it is somewhat far from its parent body (and it has a parent))
        if (parent.parent != null && parent.orbital_radius * view.currentZoom < 5f) //TODO: expensive to convert
            return;
        
        //dont draw if off screen
        Vector2 center = view.GetScreenCoordinate(parent.position);
        Vector2 rect = view.GetViewportRect().Size;
        float d_radius = display_radius();
        if ((center.x - d_radius > rect.x) || (center.x + d_radius < 0f) || (center.y - d_radius > rect.y) || (center.y + d_radius < 0f))
            return;
        
        //we are go to draw the circle
        const int points = 64;
        const double delta = Math.PI * 2.0 / ((double)points);
        
        Vector2[] circle = new Vector2[points];
        Color[] colors = new Color[] { color };
        
        for (int i = 0; i < points; i++)
            circle[i] = center + new Vector2((float)(Math.Cos(delta * i) * d_radius), (float)(Math.Sin(delta * i) * d_radius));
        
        view.DrawPolygon(circle, colors, antialiased:true);
    }
    
    public bool ClickEvent(InputEventMouseButton mouse)
    {
        Vector2 own_position = view.GetScreenCoordinate(parent.position);
        if ((mouse.Position - view.GetScreenCoordinate(parent.position)).Length() < (display_radius() + 1f)) //epsilon of 1
        {
            double deltaT = (System.DateTime.UtcNow - last_click).TotalSeconds;
            GD.Print(deltaT);
            last_click = System.DateTime.UtcNow;
            
            view.camera_position = parent.position;
            view.Update();
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
