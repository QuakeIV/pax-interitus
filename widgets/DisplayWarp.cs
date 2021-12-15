using Godot;
using System;

public class DisplayWarp : Button
{
    // Declare member variables here. Examples:
    // private int a = 2;
    // private string b = "text";
    
    // warp is now exponent of 2
    public long current_warp = 1;
    private const long min_warp = -10; // 1/1024 warp
    private const long max_warp = 24; // x16777216 warp or 16 megawarp

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        
    }

    public void WarpUpdate()
    {
        Universe.time_warp = current_warp;
        
        if (current_warp < 0)
            Text = "1/" + Math.Pow(2.0,-1*current_warp) + "x";
        else if (current_warp < 10)
            Text = Math.Pow(2.0,current_warp) + "x";
        else if (current_warp < 20)
            Text = Math.Pow(2.0,current_warp-10) + "kx";
        else
            Text = Math.Pow(2.0,current_warp-20) + "Mx";
    }

    private void _on_ReduceWarp_pressed()
    {
        if (current_warp > min_warp)
            current_warp--;
        WarpUpdate();
    }


    private void _on_IncreaseWarp2_pressed()
    {
        if (current_warp < max_warp)
            current_warp++;
        WarpUpdate();
    }
    
    private void _on_warp1_pressed()
    {
        current_warp = 0;
        WarpUpdate();
    }


    private void _on_warp1k_pressed()
    {
        current_warp = 10;
        WarpUpdate();
    }


    private void _on_warp1Mx_pressed()
    {
        current_warp = 20;
        WarpUpdate();
    }


    private void _on_warpmax_pressed()
    {
        current_warp = max_warp;
        WarpUpdate();
    }
}
