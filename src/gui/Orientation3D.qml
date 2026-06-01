import QtQuick
import QtQuick3D

Item {
    id: root
    width: 300
    height: 300

    property real modelRoll: 0.0
    property real modelPitch: 0.0
    property real modelYaw: 0.0

    View3D {
        id: view
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#0d1117" // Matches GitHub Dark background
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            id: camera
            z: 5.0
            y: 2.0
            eulerRotation.x: -20
        }

        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
            ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
            brightness: 1.5
        }

        Model {
            id: cansatModel
            // By default, this uses the local cansat.obj. 
            // The user will change this to "file:cansat.glb" later.
            source: "file:cansat.obj"
            scale: Qt.vector3d(0.5, 0.5, 0.5)

            // Map roll, pitch, yaw to eulerRotation. 
            // Depending on coordinate systems, this may need tweaking.
            eulerRotation.x: root.modelPitch
            eulerRotation.y: root.modelYaw
            eulerRotation.z: root.modelRoll

            materials: [
                PrincipledMaterial {
                    baseColor: "white"
                    metalness: 0.1
                    roughness: 0.5
                }
            ]
        }
    }
}
