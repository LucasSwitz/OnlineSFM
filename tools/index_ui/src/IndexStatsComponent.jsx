import React, { Component } from "react";
import FeatureListComponent from "./FeatureListComponent";

class IndexStatsComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      score: props.score,
      features: [],
    };
    this.featureSelected = props.featureSelected;
  }
  render() {
    return (<div>
      <p>Score: {this.state.score}</p>
      <FeatureListComponent
        features={this.state.features}
        onFeatureSelect={this.featureSelected}
      ></FeatureListComponent>
    </div>);
  }
}

export default IndexStatsComponent;
