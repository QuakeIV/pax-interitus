using Godot;
using System;

public static class Universe
{
    public static long time = 0;
    
    //eventually, negative values should represent fractional warp
    public static long time_warp = 1;
    
    public static void UpdateTime(float delta)
    {
        time += ((long)(delta*1000)) * time_warp;
    }

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}
