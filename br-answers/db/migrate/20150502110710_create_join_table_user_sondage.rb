class CreateJoinTableUserSondage < ActiveRecord::Migration
  def change
    create_join_table :users, :sondages do |t|
      t.index :user_id
      t.index :sondage_id
    end
  end
end
