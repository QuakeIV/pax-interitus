using Godot;
using System;

public class universecontainerdriver : ViewportContainer
{
    // Declare member variables here. Examples:
    // private int a = 2;
    // private string b = "text";
    
    Renderer render;

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        render = GetNode<Viewport>("Viewport").GetNode<Renderer>("Renderer");
        VisualServer.SetDefaultClearColor(new Color(0,0,0));
    }

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}
