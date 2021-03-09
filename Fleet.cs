using Godot;
using System;

//represents a formation of ships
public class Fleet
{
    public Transform transform = new Transform();
    
    public String name = "NAME UNSET";
    
    public static Renderer view;
    
    Color text_color = new Color(0,1f,0);
    
    public Fleet(String n)
    {
        name = n;
    }
    
    public Fleet(String n, FixedV2D p)
    {
        transform.position = p;
        name = n;
    }
    
    public void Draw()
    {
        // draw fleet name
        view.AddString(transform.position, name, text_color);
        
        // draw fleet position
        
        //dont draw if off screen
        Vector2 center = view.GetScreenCoordinate(transform.position);
        Vector2 rect = view.GetViewportRect().Size;
        if ((center.x - 5.0f > rect.x) || (center.x + 5.0f < 0f) || (center.y - 5.0f > rect.y) || (center.y + 5.0 < 0f))
            return;
        
        //we are go to draw the circle
        const int points = 32;
        const double delta = Math.PI * 2.0 / ((double)points);
        
        Vector2[] circle = new Vector2[points];
        Color[] colors = new Color[] { text_color };
        
        for (int i = 0; i < points; i++)
            circle[i] = center + new Vector2((float)(Math.Cos(delta * i) * 5.0f), (float)(Math.Sin(delta * i) * 5.0f));
        
        view.DrawPolygon(circle, colors, antialiased:true);
    }
}
