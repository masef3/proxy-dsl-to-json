package main

import (
	"encoding/json"
	"os"
	"log"
)

type splitArg struct {
	IP          string `json:"url"`
	Possibility int    `json:"possibility"`
}


type caseArg struct {
	CaseName string `json:"case"`
	Function string `json:"function"`
	Targets json.RawMessage `json:"targets"`

	SplitArgs []splitArg
	Target string
	Block []string 
}


type program struct {
	Port int `json:"port,omitempty"`
	Rules []caseArg `json:"rules"`
}


func parse(json_filename string) {
	msg := "Something went wrong while parsing json"
	content, err := os.ReadFile(json_filename)
	if err != nil {
		log.Fatal(msg)
	}
	
	var programOut program 
	err = json.Unmarshal(content, &programOut)
	if err != nil {
		log.Fatal(msg)
	}

	givenPort := programOut.Port
	if givenPort == 0 {
		programOut.Port = 8080
	}

	for i := range programOut.Rules {
		curr_rule := &programOut.Rules[i]

		switch curr_rule.Function {
		case "split":
			err = json.Unmarshal(curr_rule.Targets, &curr_rule.SplitArgs)
			if err != nil {
				log.Fatal(msg)
			}
		case "target":
			err = json.Unmarshal(curr_rule.Targets, &curr_rule.Target)
			if err != nil {
				log.Fatal(msg)
			}
		case "block":
			err = json.Unmarshal(curr_rule.Targets, &curr_rule.Block)
			if err != nil {
				log.Fatal(msg)
			}
		}
	}
}

