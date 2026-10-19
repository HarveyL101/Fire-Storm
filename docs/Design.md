# Design

This will contain details regarding system architecture, including trade-offs made and current limitations of the software alongside expected failure behaviour/ handling.

![System Flow Chart](/imgs/flow-chart.png)
> System Flow Chart

Above is an initial intepretation of what this system must represent, error checking/ handling will be implemented around the processes described in the chart to ensure accurate parsing and manipulation of data.

## System Archictecture

After reviewing the System Flow Chart, an appropriate structure for this case seems to be a combination of the following sub-components:

### Network Handler

![Network Layer](/imgs/network-layer.png)
> Network Layer

This layer handles the bulk of the data moving through the system, checking & filtering out the majority of network traffic via port forwarding to `PORT 5744`. Any headers containing the CTFS tag would then be passed onto the state machine.

### State Machine

### Test Handler

network layer/ handler for routing traffic, a state machine to parse headers, map data and construct the PDF