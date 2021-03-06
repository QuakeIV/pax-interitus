using Godot;
using System;
using System.Collections.Generic;


// planet, star, or other celestial objects (stars and planets probably to be split later)
public class Celestial
{
    public static Renderer view;
    public OrbitObject parent;
    
    public ulong radius;
    public Color color;
    
    public Celestial(ulong r, Color c)
    {
        radius = r;
        color = c;
    }
    
    public Celestial(ulong r)
    {
        radius = r;
        color = new Color(9.0f/255.0f, 76.0f/255.0f, 13.0f/255.0f);
    }
    
    public void Draw()
    {
        if (parent.parent == null || parent.orbital_radius * view.currentZoom > 5f) //TODO: bit expensive to constantly convert
        {
            //only bother drawing if it is somewhat far from its parent body)
            float display_radius = radius*view.currentZoom;
            //enforce minimum dot radius
            if (display_radius < 5.0f)
                display_radius = 5.0f;
            
            //TODO: draw circle looks crusty as shit, replace
            view.DrawCircle(view.GetScreenCoordinate(parent.position), display_radius, color);
        }
    }
}

public class Renderer : Godot.Node2D
{
    private FixedV2D camera_position = new FixedV2D();
    
    private FixedV2D test_star = new FixedV2D(0, 0);
    // Declare member variables here. Examples:
    // private int a = 2;
    // private string b = "text";

    OrbitObject sol;
    
    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        OrbitObject.view = this;
        Celestial.view = this;
        
        //sun color picked by flocco
        Celestial cel = new Celestial(695700000000, new Color(226f/255f, 223f/255f, 24f/255f));
        sol = new OrbitObject(1988500000000000, cel);
        
        cel = new Celestial(6378100000, new Color(9f/255f, 76f/255f, 13f/255f));
        OrbitObject earth = sol.AddChild(149598023000000, 5972200000, cel); //earth
        
        cel = new Celestial(1737400000, new Color(192f/255f, 193f/255f, 180f/255f));
        OrbitObject mun = earth.AddChild(384399000000, 73420000, cel); //moon
    }

    public Vector2 GetScreenCoordinate(FixedV2D a)
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
        //realtime for now
        sol.UpdatePosition((long)(delta*1000));
        Update();
    }

    public override void _Draw()
    {
        sol.Draw();
    }
    
    
    public float currentZoom = 0.000000000001f; //1.0f;
    // member variables here
    private Vector2 mouse1;
    private bool pressed = false;
    
    public override void _UnhandledInput(InputEvent @event)
    {
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
    }
    
    private void _on_Viewport_size_changed()
    {
        Update();
    }
}
