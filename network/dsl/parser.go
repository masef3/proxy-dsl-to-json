package main

import (
	"encoding/json"
	"os"
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
	content, err := os.ReadFile(json_filename)
	if err != nil {
		panic(err)
	}
	
	var programOut program 
	err = json.Unmarshal(content, &programOut)
	if err != nil {
		panic(err)
	}

	for i := range programOut.Rules {
		curr_rule := &programOut.Rules[i]

		switch curr_rule.Function {
		case "split":
			err = json.Unmarshal(curr_rule.Targets, &curr_rule.SplitArgs)
			if err != nil {
				panic(err)
			}
		case "target":
			err = json.Unmarshal(curr_rule.Targets, &curr_rule.Target)
			if err != nil {
				panic(err)
			}
		case "block":
			err = json.Unmarshal(curr_rule.Targets, &curr_rule.Block)
			if err != nil {
				panic(err)
			}
		}
	}

}

