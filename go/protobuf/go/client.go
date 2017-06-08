package main

import (
    "os"
    "fmt"
    "net"
    "proto"
)



func main() {  
    regMessage := &RegMessage{  
        Id:       proto.Int32(10001),
        Username: proto.String("vicky"),
        Password: proto.String("123456"),
        Email:    proto.String("eclipser@163.com"),
    }

    buffer, err := proto.Marshal(regMessage)
    if err != nil {
        fmt.Printf("failed: %s\n", err)
        return
    }

    pTCPAddr, err := net.ResolveTCPAddr("tcp", "192.168.0.110:7070")
    if err != nil {
        fmt.Fprintf(os.Stderr, "Error: %s\n", err.Error())
        return
    }

    pTCPConn, err := net.DialTCP("tcp", nil, pTCPAddr)
    if err != nil {
        fmt.Fprintf(os.Stderr, "Error: %s\n", err.Error())
        return
    }

    pTCPConn.Write(buffer)

    return
}

// go run client.go RegMessage.pb.go