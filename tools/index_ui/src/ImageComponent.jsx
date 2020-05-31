import React, { Component } from "react";

class ImageComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      image_id: props.image_id,
      reconstruction_id: props.reconstruction_id,
      image_fetched: false,
      image_path: "",
    };
  }
  componentDidMount() {
    fetch(
      `http://localhost:8000/images/${this.state.reconstruction_id}/${this.state.image_id}`
    )
      .then((res) => res.json())
      .then((result) => {
        this.setState({
          image_path: result.path,
          image_fetched: true,
        });
      });
  }
  render() {
    if (!this.state.image_fetched) {
      return <h2 className="image_panel_title">{this.state.image_id}</h2>;
    } else {
      return (
        <div className="image_panel">
          <img className="image_panel_image" src={this.state.image_path}></img>
          <h2 className="image_panel_title">{this.state.image_id}</h2>
        </div>
      );
    }
  }
}

export default ImageComponent;
