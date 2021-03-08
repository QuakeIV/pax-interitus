using Godot;
using System;
using System.Collections.Generic;

public class Renderer : Godot.Node2D
{
    public FixedV2D camera_position = new FixedV2D();
    
    public Font font;

    OrbitObject sol;
    
    bool paused = false;
    
    public float currentZoom = 0.000000000001f; //1.0f;
    // member variables here
    private Vector2 mouse1;
    private Vector2 click1;
    private bool pressed = false;
    
    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
//        Button butt = GetTree().CurrentScene.GetNode("MainContainer").FindNode<Button>("StartStop");

        font = ((Control)GetTree().CurrentScene).GetFont("font");
        
        // set background color
        VisualServer.SetDefaultClearColor(new Color(0,0,0));
        
        OrbitObject.view = this;
        Celestial.view = this;
        
        //sun color picked by flocco
        Celestial cel = new Celestial(695700000000, 1988500000000000U, new Color(226f/255f, 223f/255f, 24f/255f));
        sol = new OrbitObject(cel);
        
        //mercury
        cel = new Celestial(2439700000, 330110000U, new Color(192f/255f, 193f/255f, 180f/255f));
        OrbitObject mercury = sol.AddChild(57909050000000, cel);
        
        //venus
        cel = new Celestial(6051800000, 4867500000U, new Color(239f/255f, 119f/255f, 14f/255f));
        OrbitObject venus = sol.AddChild(108208000000000, cel);
        
        //earth
        cel = new Celestial(6378100000, 5972200000U, new Color(19f/255f, 33f/255f, 219f/255f));
        OrbitObject earth = sol.AddChild(149598023000000, cel);
        //moon
        cel = new Celestial(1737400000, 73420000U, new Color(192f/255f, 193f/255f, 180f/255f));
        OrbitObject mun = earth.AddChild(384399000000, cel);
        
        //mars
        cel = new Celestial(3389500000, 641710000U, new Color(242f/255f, 76f/255f, 26f/255f));
        OrbitObject mars = sol.AddChild(227939200000000, cel);
        //phobos/deimos
        cel = new Celestial(11266700, 11U, new Color(192f/255f, 193f/255f, 180f/255f));
        mars.AddChild(9376000000, cel);
        cel = new Celestial(6200000, 1U, new Color(192f/255f, 193f/255f, 180f/255f));
        mars.AddChild(23463200000, cel);
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
        Update();
    }

    // Called every frame. 'delta' is the elapsed time since the previous frame.
    public override void _PhysicsProcess(float delta)
    {
        //realtime for now
        if (!paused)
        {
            Universe.UpdateTime(17); //change this manually if the intended framerate changes, that way each tick is consistent
            sol.UpdatePosition();
        }
    }
    
    public void WriteString(FixedV2D position, String str, byte r=255, byte g=255, byte b=255)
    {
        DrawString(font, new Vector2(200,200), "test string");
    }
    
    Color interface_color = new Color(0f, 1f, 0f);
    private Vector2 scale_from = new Vector2(20f, 30f);
    private Vector2 scale_to   = new Vector2(120f, 30f);
    private Vector2 scale_text = new Vector2(20f, 28f);
    private String[] si_scale  = {"m", "", "k","M","G","T"};
    public override void _Draw()
    {
        sol.Draw();
        
        //draw scale marker
        DrawLine(scale_from, scale_to, interface_color);
        float dist = (100f/currentZoom);
        int i;
        for (i = 0; i < si_scale.Length && dist > 10000f; i++)
        {
            dist /= 1000f;
        }
        DrawString(font, scale_text, String.Format("{0:G4}{1}m", dist, si_scale[i]), interface_color);
    }
    
    public override void _UnhandledInput(InputEvent @event)
    {
        // Mouse in viewport coordinates.
        if (@event is InputEventMouseButton eventMouseButton)
        {
            if ((ButtonList)eventMouseButton.ButtonIndex == ButtonList.Left)
            {
                if (eventMouseButton.Pressed)
                {
                    mouse1 = eventMouseButton.Position;
                    click1 = mouse1;
                    pressed = true;
                }
                else
                {
                    pressed = false;
                    if ((click1 - eventMouseButton.Position).Length() < 5.0)
                    {
                        //pass mouse events to the system if it doesnt look like it was intended to pan the screen
                        bool caught = sol.ClickEvent(eventMouseButton);
                        if (caught)
                            GD.Print("caught mouse click");
                        else
                            GD.Print("MISSED mouse click");
                    }
                }
            }
            else if ((ButtonList)eventMouseButton.ButtonIndex == ButtonList.Right)
            {
                bool caught = sol.AltClickEvent(eventMouseButton);
                if (caught)
                    GD.Print("caught mouse click");
                else
                    GD.Print("MISSED mouse click");
            }
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
            Update();
        }
        
        if (Input.IsActionPressed("zoom_in"))
        {
            currentZoom/=0.8f;
            Update();
        }
    }
       
    private void _on_StartStop_StartStopPress(StartStop button)
    {
        paused = button.stop;
    }
}
