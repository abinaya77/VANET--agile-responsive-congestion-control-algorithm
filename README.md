# abi-congestion-control-algorithm-for-VANET
PROBLEM STATEMENT
  1)False Congestion Notification 
  2)Flat Rate Reduction
  3)Recovery ; same linear pattern

  ![image](https://github.com/abinaya77/congestion-control-algorithm-for-VANET/assets/106723283/4a24bfa6-9525-47dc-9fd6-d2391ee5d2f7)

  
PROPOSED WORK:
The problem in other TCP is that it cannot identify whether the congestion is due to traffic or network errors. when some data is sent from the host to the server ,a piece of data have to wait a long time before it reaches the receiver and for the acknowledgement to come back . Due to delay in acknowledgement ,congestion is notified where  other TCP ‘s flatly reduces the sending rate by half without considering the TCP flows growth rate for both most utilized and least utilized TCP flows.
In recovery phase , linear window growth pattern is followed for both most utilized and least utilized TCP flows which results in unfairness. In this project a new algorithm is created which gives better results when compared with other existing TCP algorithm.
 In the slow start phase, a new parameter named Window Utility Level (WUL) is fixed which is half the receiver buffer limit. Then the  exponential growth pattern occurs until the window reaches the receiver buffer limit and then the linear increment pattern follows until congestion occurs. Once congestion is notified then new reduction parameter WUL is considered for reduction of the window .In this algorithm ,other than Flat Rate reduction of the window ,the congested window is compared with the WUL if the window is lesser than WUL then the reduction is one fourth of the congested window or the reduction is half of the congested window.
![image](https://github.com/abinaya77/congestion-control-algorithm-for-VANET/assets/106723283/55ae7c01-014a-4450-8ac3-000387eba58e)

