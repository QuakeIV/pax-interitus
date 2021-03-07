using Godot;
using System;

public class DisplayWarp : Button
{
    // Declare member variables here. Examples:
    // private int a = 2;
    // private string b = "text";
    
    //eventually maybe negative can be fractional warp?
    public long current_warp = 1;
    private const long max_warp = 16777216;

    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        
    }

    public void WarpUpdate()
    {
        Universe.time_warp = current_warp;
        
        if (current_warp < 1024)
            Text = current_warp + "x";
        else if (current_warp < 1048576)
            Text = current_warp/1024 + "kx";
        else
            Text = current_warp/(1024 * 1024) + "Mx";
        
    }

    private void _on_ReduceWarp_pressed()
    {
        if (current_warp > 1)
            current_warp /= 2;
        WarpUpdate();
    }


    private void _on_IncreaseWarp2_pressed()
    {
        if (current_warp < max_warp)
            current_warp *= 2;
        WarpUpdate();
    }
    
    private void _on_warp1_pressed()
    {
        current_warp = 1;
        WarpUpdate();
    }


    private void _on_warp1k_pressed()
    {
        current_warp = 1024;
        WarpUpdate();
    }


    private void _on_warp1Mx_pressed()
    {
        current_warp = 1024*1024;
        WarpUpdate();
    }


    private void _on_warpmax_pressed()
    {
        current_warp = max_warp;
        WarpUpdate();
    }
}
