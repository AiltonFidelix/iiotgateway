
# Modbus Request Structure

This document describes the `JSON` structure used to perform **Modbus read or write operations**.

A request may contain **multiple devices**, but **all operations within the same request must be of the same type**:

- **Read request** – reads registers or coils.
- **Write request** – writes values to registers or coils.

A request **cannot mix read and write operations**.

---

## Request Format

```json
{
  "devices": [
    {
      "address": 140,
      "type": "HoldingRegisters",
      "startRegister": 0,
      "numberOfEntries": 2
    }
  ]
}
```

### Root Object

| Field     | Type  | Description                                                                             |
| --------- | ----- | --------------------------------------------------------------------------------------- |
| `devices` | array | List of Modbus device operations. Multiple devices can be included in a single request. |

### Device Object

Each element in the `devices` array represents an operation for a specific Modbus device.

| Field             | Type          | Required   | Description                                                                             |
| ----------------- | ------------- | ---------- | --------------------------------------------------------------------------------------- |
| `address`         | number        | Yes        | Modbus slave address of the target device.                                              |
| `type`            | string        | Yes        | Modbus register type (`HoldingRegisters`, `InputRegisters`, `Coils`, `DiscreteInputs`). |
| `startRegister`   | number        | Yes        | Starting register or coil address.                                                      |
| `numberOfEntries` | number        | Yes        | Number of registers or coils to read or write.                                          |
| `values`          | array<number> | Write only | Values to write. Required for write requests.                                           |

---

## Reply Format

```json
{
  "datetime": "03/11/2025 22:00:00.000",
  "devices": [
    {
      "address": 140,
      "registers": [
        { 
          "register": 0,
          "value": 64
        },
        { 
          "register": 1,
          "value": 1023
        }
      ]
    }
  ]
}
```

### Root Object

| Field      | Type   | Description                                                                      |
| ---------- | ------ | -------------------------------------------------------------------------------- |
| `datetime` | string | Date and time of the request.                                                    |
| `devices`  | array  | List of Modbus device reads. Multiple devices can be returned in a single reply. |

### Device Object

Each element in the `devices` array represents a reading result for a specific Modbus device.

| Field       | Type   | Description                                 |
| ----------- | ------ | ------------------------------------------- |
| `address`   | number | Modbus slave address of the reading device. |
| `registers` | array  | List of reading values object.              |

### Register Object

Each element in the `registers` array represents a reading for a specific Modbus register or coil.

| Field      | Type   | Description                           |
| ---------- | -------| ------------------------------------- |
| `register` | number | Modbus register or coil address.      |
| `value`    | number | Value from a modbus register or coil. |

---

## Read Request

To perform a **read operation**, the `values` field must **not be included**.

### Example

```json
{
  "devices": [
    {
      "address": 140,
      "type": "HoldingRegisters",
      "startRegister": 0,
      "numberOfEntries": 25
    }
  ]
}
```

### Explanation

This request reads:

- **Device address:** `140`
- **Register type:** `HoldingRegisters`
- **Start register:** `0`
- **Number of registers:** `25`

Registers **0–24** will be read from the device.

---

## Write Request

To perform a **write operation**, the `values` array must be provided.

The number of values should match `numberOfEntries`.

### Example

```json
{
  "devices": [
    {
      "address": 140,
      "type": "HoldingRegisters",
      "startRegister": 1,
      "numberOfEntries": 1,
      "values": [1]
    }
  ]
}
```

### Explanation

This request writes:

- **Device address:** `140`
- **Register type:** `HoldingRegisters`
- **Target register:** `1`
- **Values written:** `[1]`

Register **1** will receive the value **1**.

---

## Writing Multiple Registers

Multiple registers can be written in a single operation using the `values` array.

### Example

```json
{
  "devices": [
    {
      "address": 140,
      "type": "HoldingRegisters",
      "startRegister": 10,
      "numberOfEntries": 3,
      "values": [10, 20, 30]
    }
  ]
}
```

### Registers Written

| Register | Value |
| -------- | ----- |
| 10       | 10    |
| 11       | 20    |
| 12       | 30    |

---

## Multiple Devices in a Request

A single request can target multiple devices **as long as all operations are the same type**.

### Read example

```json
{
  "devices": [
    {
      "address": 11,
      "type": "HoldingRegisters",
      "startRegister": 0,
      "numberOfEntries": 1
    },
    {
      "address": 12,
      "type": "HoldingRegisters",
      "startRegister": 5,
      "numberOfEntries": 2
    }
  ]
}
```

This request reads registers from **two devices** in a single request.

The same device **can appear multiple times** to `read` or `write` **non-contiguous** register ranges.

---

# Notes

- A request must be either `read` or `write`.
- Mixing `read` and `write` operations in the same request **is not allowed.**
- When writing, the length of `values` should match `numberOfEntries`.
- Devices are processed **in the order they appear** in the `devices` array.