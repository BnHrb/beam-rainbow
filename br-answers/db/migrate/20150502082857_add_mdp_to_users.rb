class AddMdpToUsers < ActiveRecord::Migration
  def change
    add_column :users, :mdp, :string
  end
end
