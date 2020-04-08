import bpy
from bpy.types import Panel
import os

from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator
import xml.etree.ElementTree as ET
import ntpath

def indentXml(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indentXml(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

def PrintModelFile(filepath):
    xmlTree = ET.ElementTree()
    modelRoot = ET.Element('modelRoot')
    modelElement = ET.SubElement(modelRoot, "model")

    # resolving filepath
    temp_filePath = filepath.lower()
    dataPathSearchString = 'zmasher/data'
    dataPathIndex = temp_filePath.find(dataPathSearchString)

    if dataPathIndex == -1:
        dataPathSearchString = 'zmasher\\data'
        dataPathIndex = temp_filePath.find(dataPathSearchString)
    if dataPathIndex == -1:
        raise Exception("Couldn't find ZMasher/Data in the filePath")

    dataPath = temp_filePath[dataPathIndex + len(dataPathSearchString)+1: len(temp_filePath)-4]

    print ('dataPath:', dataPath)
    modelElement.set("filepath", dataPath)

    filePathWithoutEnding = filepath[:len(filepath)-4]
    modelFilePath = filePathWithoutEnding + '.model'
    print ('modelFilePath:', modelFilePath)

    indentXml(modelRoot)
    xmlTree._setroot(modelRoot)
    xmlTree.write(modelFilePath,)

def ZM_PrintModel(filepath):
    temp_filePath = filepath.lower()
    if temp_filePath.find('zmasher/data') == -1 and\
        temp_filePath.find('zmasher\\data') == -1:
        print ("File needs to be under ZMasher/Data")
        return

    selection = bpy.context.selected_objects
    if len(selection) > 1:
        print ("Only select one object at a time")
        return
    elif len(selection)==0:
        print ("select at least one object before exporting")
        return

    #current_work_dir = os.path.dirname(bpy.data.filepath)
    #model_dir = current_work_dir + "../../"
    #model_file_name =  ntpath.basename(bpy.data.filepath)
    #try:
    #    os.mkdir(model_dir)
    #except OSError:
    #    print ("Couldn't lol")
    #else:
    #    print ("yes lol")
    
    scene = bpy.context.scene

    bpy.ops.object.select_all(action='DESELECT')
    
    obj = selection[0]
    
    obj.select = True
    # some exporters only use the active object
    scene.objects.active = obj

    #model_full_file_path = model_dir + model_file_name + ".fbx"
    #bpy.ops.export_scene.fbx(filepath=model_full_file_path, use_selection=True)
    bpy.ops.export_scene.fbx(filepath=filepath, use_selection=True)

    obj.select = False
    PrintModelFile(filepath)
    print("written:", filepath)
    return {'FINISHED'}

class ZMasherModelExportOperator(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "ui.zmashermodelexportoperator"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Model to the game"

    filename_ext = ".fbx"
    
    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting = BoolProperty(
            name="Example Boolean",
            description="Example Tooltip",
            default=True,
            )

    type = EnumProperty(
            name="Example Enum",
            description="Choose between two items",
            items=(('OPT_A', "First Option", "Description one"),
                   ('OPT_B', "Second Option", "Description two")),
            default='OPT_A',
            )

    def execute(self, context):
        return ZM_PrintModel(self.filepath)

class ZMasherExportPanel(Panel):
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'TOOLS'
    bl_label = 'ZMasher Model Export'
    bl_context = 'objectmode'
    bl_category = 'ZMasher'
    
    def draw(self, context):
        layout = self.layout
        layout.operator("ui.zmashermodelexportoperator", text='Export Model')
        
def register():
    bpy.utils.register_class(ZMasherExportPanel)
    bpy.utils.register_class(ZMasherModelExportOperator)
    
def unregister():
    bpy.utils.unregister_class(ZMasherExportPanel)
    bpy.utils.unregister_class(ZMasherModelExportOperator)
    
if __name__ == '__main__':
    register()

