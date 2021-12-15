using Godot;
using System;

public static class Universe
{
    public static long time = 0;
    
    //warp is now exponent of 2
    public static long time_warp = 0;
    private static float time_remainder;
    
    public static void UpdateTime(float delta)
    {
        delta *= (float)(1000.0*(Math.Pow(2.0,time_warp)));
        long delta_ms = (long)delta;
        time_remainder += (delta - delta_ms);
        
        if (time_remainder > 1.0)
        {
            delta_ms += 1;
            time_remainder -= 1.0f;
        }
        
        time += delta_ms;
    }

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}
