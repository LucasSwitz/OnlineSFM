import React, { Component } from "react";

class SearchBarComponent extends Component {
  constructor(props) {
    super(props);
    this.state = {
      reconstruction_id: "",
      image_id: "",
    };
    this.handleImageChange = this.handleImageChange.bind(this);
    this.handleReconstructionChange = this.handleReconstructionChange.bind(
      this
    );
    this.handleSubmit = this.handleSubmit.bind(this);
    this.onSearch = props.onSearch;
  }

  handleReconstructionChange(event) {
    this.setState({ reconstruction_id: event.target.value });
  }

  handleImageChange(event) {
    this.setState({ image_id: event.target.value });
  }

  handleSubmit(event) {
    this.onSearch(this.state.reconstruction_id, this.state.image_id);
    event.preventDefault();
  }

  render() {
    return (
      <form className="search-bar" onSubmit={this.handleSubmit}>
        <label>
          Reconstruction:
          <input
            type="text"
            value={this.state.reconstruction_id}
            onChange={this.handleReconstructionChange}
          />
        </label>
        <label>
          Image:
          <input
            type="text"
            value={this.image_id}
            onChange={this.handleImageChange}
          />
        </label>
        <input type="submit" value="Submit" />
      </form>
    );
  }
}

export default SearchBarComponent;
