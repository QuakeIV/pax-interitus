using Godot;
using System;

public class testwindow : WindowDialog
{
    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        
    }
    
    private void _on_TestButton_pressed()
    {
        PopupCentered();
    }
}
