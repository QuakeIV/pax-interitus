using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public class Renderer : Godot.Node2D
{
    public FixedV2D camera_position = new FixedV2D();
    
    public Font font;

    OrbitObject sol;
    List<Fleet> fleets = new List<Fleet>();
    
    bool paused = false;
    
    //TODO: bracket zooming
    public float currentZoom = 0.000000000001f; //1.0f;
    
    // member variables here
    private Vector2 mouse1;
    private Vector2 click1;
    private bool pressed = false;
    
    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        font = ((Control)GetTree().CurrentScene).GetFont("font");
        
        // set background color
        VisualServer.SetDefaultClearColor(new Color(0,0,0));
        
        OrbitObject.view  = this;
        Celestial.view    = this;
        Fleet.view        = this;
        
        //sun color picked by flocco
        Celestial cel = new Celestial(695700000000, 1988500000000000U, 226, 223, 24);
        sol = new OrbitObject(cel);
        
        //mercury
        cel = new Celestial(2439700000, 330110000U, 192, 193, 180);
        OrbitObject mercury = sol.AddChild(57909050000000, cel);
        
        //venus
        cel = new Celestial(6051800000, 4867500000U, 239, 119, 14);
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
        
        
        
        // fleet testing
        Fleet f = new Fleet("ree", new FixedV2D(1000000000000,1000000000000));
        fleets.Add(f);
        f = new Fleet("scree", new FixedV2D(-1000000000000,-1000000000000));
        fleets.Add(f);
        f = new Fleet("shrimp");
        fleets.Add(f);
    }

    public Vector2 GetScreenCoordinate(FixedV2D a)
    {
        FixedV2D v = a - camera_position;
        Vector2 screen = GetViewportRect().Size;
        screen.x = screen.x / 2.0f;
        screen.y = screen.y / 2.0f;
        
        Vector2 zoom_pos = new Vector2((float)Math.Round((v.x * currentZoom) + screen.x), (float)Math.Round((v.y * currentZoom) + screen.y));
        return zoom_pos;
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
            Universe.UpdateTime(delta);
            sol.UpdatePosition(); //TODO: this is just a lazy way to update the solar system
        }
    }
    
    private List<Tuple<Vector2,String,Color>> strings = new List<Tuple<Vector2,String,Color>>();
    public void AddString(FixedV2D position, String str, byte r=255, byte g=255, byte b=255)
    {
        AddString(position, str, new Color(r/255f, g/255f, b/255f));
    }
    public void AddString(FixedV2D position, String str, Color color)
    {
        Vector2 pos = GetScreenCoordinate(position);
        
        Tuple<Vector2,String,Color> tup = new Tuple<Vector2,String,Color>(pos, str, color);
        strings.Add(tup);
    }
    
    public void DrawStrings()
    {
        //sort by y coordinate to clean up how it displays
        strings = strings.OrderBy(o => o.Item1.y).ToList();
        List<Tuple<Vector2,String,Color>> remainder = new List<Tuple<Vector2,String,Color>>();
        while (strings.Count > 0)
        {
            Tuple<Vector2,String,Color> tup = strings[0];
            strings.RemoveAt(0);
            //correct position to not encroach on associated dot (TODO: this would vary depending on application, hardcode will frequently fail)
            Vector2 pos = tup.Item1 + new Vector2(5,-5);
            DrawString(font, pos, tup.Item2, tup.Item3);
            pos.y += (font.GetHeight() + 2);
            while (strings.Count > 0)
            {
                Tuple<Vector2,String,Color> next = strings[0];
                strings.RemoveAt(0);
                if ((next.Item1 - tup.Item1).Length() < 6.0f)
                {
                    DrawString(font, pos, next.Item2, next.Item3);
                    pos.y += (font.GetHeight() + 2);
                }
                else
                    remainder.Add(next);
            }
            List<Tuple<Vector2,String,Color>> temp = strings;
            strings = remainder;
            remainder = temp;
        }
    }
    
    Color interface_color = new Color(0f, 1f, 0f);
    private Vector2 scale_from = new Vector2(20f, 30f);
    private Vector2 scale_to   = new Vector2(120f, 30f);
    private Vector2 scale_text = new Vector2(20f, 28f);
    private String[] si_scale  = {"m", "", "k","M","G","T"};
    public override void _Draw()
    {
        sol.Draw();
        foreach (Fleet f in fleets)
        {
            f.Draw();
        }
        DrawStrings();
        
        //draw scale marker
        DrawLine(scale_from, scale_to, interface_color);
        float dist = (100f/currentZoom);
        int i;
        for (i = 0; i < (si_scale.Length - 1) && dist > 10000f; i++)
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
