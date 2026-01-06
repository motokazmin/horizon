#pragma once

#include <QString>
#include <QMetaType>

namespace HorizonUTM {

/**
 * @brief Enumeration of possible machine states
 */
enum class MachineState {
    Disconnected,    ///< Not connected to hardware
    Idle,            ///< Connected but not running
    Running,         ///< Test in progress
    Paused,          ///< Test paused
    Stopping,        ///< Test stopping
    Error            ///< Error state
};

/**
 * @brief Convert MachineState to string
 */
inline QString machineStateToString(MachineState state) {
    switch (state) {
        case MachineState::Disconnected: return "Disconnected";
        case MachineState::Idle:         return "Idle";
        case MachineState::Running:      return "Running";
        case MachineState::Paused:       return "Paused";
        case MachineState::Stopping:     return "Stopping";
        case MachineState::Error:        return "Error";
        default:                         return "Unknown";
    }
}

/**
 * @brief Get color for machine state (for UI)
 */
inline QString machineStateColor(MachineState state) {
    switch (state) {
        case MachineState::Disconnected: return "#64748b"; // slate-500
        case MachineState::Idle:         return "#22c55e"; // green-500
        case MachineState::Running:      return "#3b82f6"; // blue-500
        case MachineState::Paused:       return "#f59e0b"; // amber-500
        case MachineState::Stopping:     return "#f97316"; // orange-500
        case MachineState::Error:        return "#ef4444"; // red-500
        default:                         return "#64748b";
    }
}

} // namespace HorizonUTM

// Register with Qt meta-object system
Q_DECLARE_METATYPE(HorizonUTM::MachineState)
