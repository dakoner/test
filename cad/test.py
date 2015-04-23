Gui.activateWorkbench("PartDesignWorkbench")
import FreeCAD
FreeCAD.open("/home/dek/box/shapeoko-x-axis-limit-switch-mount.fcstd")
App.setActiveDocument("shapeoko_x_axis_limit_switch_mount")
App.ActiveDocument=App.getDocument("shapeoko_x_axis_limit_switch_mount")
Gui.ActiveDocument=Gui.getDocument("shapeoko_x_axis_limit_switch_mount")

App.getDocument("shapeoko_x_axis_limit_switch_mount").Sketch.Placement=App.Placement(App.Vector(16.91,0,15),App.Rotation(0.5,0.5,0.5,0.5))

Gui.activeDocument().setEdit('Pocket',0)
Gui.activeDocument().resetEdit()
App.getDocument("shapeoko_x_axis_limit_switch_mount").Sketch002.Placement=App.Placement(App.Vector(0,0,-100),App.Rotation(0,0,0,1))

Gui.SendMsgToActiveView("ViewFit")
