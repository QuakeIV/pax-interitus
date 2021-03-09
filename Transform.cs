using Godot;
using System;

public class Transform
{
    public FixedV2D position = new FixedV2D();
    
    public virtual void UpdatePosition()
    {
        GD.Print("default update position method");
    }
}
