using Godot;
using System;

public class StartStop : Button
{
    // Declare member variables here. Examples:
    // private int a = 2;
    // private string b = "text";
    
    public bool stop = false; //start out set to be 'false'
    
    [Signal]
    delegate void StartStopPress(StartStop button);

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        
    }

    //called when pressed
    public override void _Pressed()
    {
        if (stop)
            setStart();
        else
            setStop();
        
        EmitSignal(nameof(StartStopPress), this);
    }
    
    public void setStart()
    {
        stop = false;
        Text = "PAUSE";
    }
    
    public void setStop()
    {
        stop = true;
        Text = "PAUSED";
    }

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}
